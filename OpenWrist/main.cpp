// Evan Pezent (epezent@rice.edu, evanpezent.com)
// 06/2017

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <chrono>
#include <cstdio>
#include <csignal>
#include <windows.h>
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include "Daq.h"
#include "Q8USB.h"
#include "OpenWrist.h"
#include "OpenWristShare.h"
#include <vector>
#include "util.h"

static bool        stop = false;                          /* flag used to stop the controller */
static const int   frequency = 1000;                  /* controller loop rate (Hz) */

namespace po = boost::program_options;

/* Ctrl-C handler */
static void signal_handler(int signum) {
	stop = true;
}

int main(int argc, char * argv[]) {

	/* register signal SIGINT and SIGBREAK with signal handler */
	signal(SIGINT, signal_handler);
	signal(SIGBREAK, signal_handler);

	/* set up program options */
	po::options_description desc("Available Options");
	desc.add_options()
		("help", "produces help message")
		("zero", "zeros encoder counts on startup")
		("calibrate", "calibrate OpenWrist zero position on start-up")
		("disable_q8", "disables Q8 USB initialization for debugging purposes");

	po::variables_map var_map;
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);

	if (var_map.count("help")) {
		std::cout << desc << "\n";
		return -1;
	}

	/* instatiate Q8 USB */
    std::string id = "0";
	uint_vec  ai_channels = { 0, 1, 2 };
	uint_vec  ao_channels = { 0, 1, 2 };
	uint_vec  di_channels = { 0, 1, 2 };
	uint_vec  do_channels = { 0, 1, 2 };
	uint_vec enc_channels = { 0, 1, 2 };
	char options[] = "ch0_mode=2;ch0_kff=0;ch0_a0=-1.382;ch0_a1=8.03;ch0_a2=0;ch0_b0=-1;ch0_b1=0;ch0_post=1000;ch1_mode=2;ch1_kff=0;ch1_a0=-1.382;ch1_a1=8.03;ch1_a2=0;ch1_b0=-1;ch1_b1=0;ch1_post=1000;ch2_mode=2;ch2_kff=0;ch2_a0=1.912;ch2_a1=18.43;ch2_a2=0;ch2_b0=-1;ch2_b1=0;ch2_post=1000;update_rate=fast;ext_int_polarity=0;convert_polarity=1;watchdog_polarity=0;ext_int_watchdog=0;use_convert=0;pwm_immediate=0;decimation=1";
	Daq *q8 = new Q8Usb(id, ai_channels, ao_channels, di_channels, do_channels, enc_channels, options);

	/* instantiate and initialize OpenWrist */
	OpenWrist ow;
	ow.bind_daq(q8);

	/* calibrate joint positions */
	if (var_map.count("calibrate")) {
		std::cout << "Press ENTER to start calibration.";
		getchar();
		ow.calibrate(stop);
	}

	/* initialize Q8 USB */
	if (!var_map.count("disable_q8")) {
		if (!q8->init()) {
			std::cout << "Terminating controller" << std::endl;
			return -1;
		}
	}
	
	/* manual zero joint positions */
	if (var_map.count("zero")) {
		ow.zero_joint_positions();
	}

	/* create OpenWristShare */
	OpenWristShare ow_share(&ow);

	/* declare controller variables */
	double_vec gravity_torques = { 0, 0, 0 };
	double_vec friction_torques = { 0, 0, 0 };

	/* request users permission to execute the controller */
	std::cout << "Press ENTER to execute the controller.";
	getchar();

	/* begin control */
	std::cout << "Executing control. Press Ctrl+C to terminate the controller." << std::endl;

	// enable OpenWrist
	ow.enable();

	/* start watchdog */
	q8->start_watchdog(0.1);

	/* start time keeping variables */
	std::chrono::high_resolution_clock::time_point time_start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point time_start_loop = time_start;
	std::chrono::high_resolution_clock::time_point time_now = time_start;
	std::chrono::nanoseconds time_elapsed_loop(0);
	std::chrono::nanoseconds time_elapsed_total(0);
	std::chrono::nanoseconds time_per_sample(1000000000 / frequency);
	std::chrono::nanoseconds time_per_sample_offset(200);
	double controller_time = 0;

	/* start the control loop */
	int sample_number = 0;
	while (stop == 0) {
		time_start_loop = std::chrono::high_resolution_clock::now();
		controller_time = (double)sample_number / frequency;
		/* reloead watchdog */
		q8->reload_watchdog();

		/* START CONTROLLER SPECIFIC CODE */
		ow.read_all();
		q8->read_analog();

		
		OpenWrist::compute_gravity_compensation_joint_torques(ow.joint_positions_, gravity_torques);
		gravity_torques[2] = 0;
		OpenWrist::compute_friction_compensation_joint_torques(ow.joint_velocities_, friction_torques);
		for (int i = 0; i < ow.num_joints_; i++)
			ow.joint_torques_[i] = gravity_torques[i] + friction_torques[i];


		ow.command_joint_torques();
		

		/* END CONTROLLER SPECIFIC CODE */
		/* log data */
        q8->log_data(controller_time);
		/* update state values over OpenWristShare */
		ow_share.update_state(controller_time);
		/* increment sample number */
		sample_number += 1;
		/* spinlock / busy wait the control loop until the loop rate has been reached */
		time_now = std::chrono::high_resolution_clock::now();
		time_elapsed_loop = time_now - time_start_loop;
		time_elapsed_total = time_now - time_start;
		while (time_elapsed_loop < time_per_sample - time_per_sample_offset) {
			time_now = std::chrono::high_resolution_clock::now();
			time_elapsed_loop = time_now - time_start_loop;
			time_elapsed_total = time_now - time_start;
		}
		time_now = std::chrono::high_resolution_clock::now();
		time_elapsed_loop = time_now - time_start_loop;
	}

	std::cout << "Ctrl-C pressed. Terminating control." << std::endl;

	/* end control and clean up */
	ow.disable();
	q8->terminate();

	return 0;
}

// Useful Links
// http://www.quanser.com/Products/quarc/documentation/q8_usb.html
// https://www.codeproject.com/KB/winsdk/console_event_handling.aspx?display=PrintAll
// https://superuser.com/questions/459609/what-does-it-do-exactly-if-i-click-in-the-window-of-cmd
// https://theboostcpplibraries.com/boost.interprocess-shared-memory