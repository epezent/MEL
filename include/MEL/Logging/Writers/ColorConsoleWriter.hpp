// MIT License
//
// MEL - Mechatronics Engine & Library
// Copyright (c) 2019 Mechatronics and Haptic Interfaces Lab - Rice University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <MEL/Logging/Writers/ConsoleWriter.hpp>

namespace mel {

//==============================================================================
// CLASS DECLARATION
//==============================================================================

template <class Formatter>
class ColorConsoleWriter : public ConsoleWriter<Formatter> {
public:

    ColorConsoleWriter(Severity max_severity = Debug) : ConsoleWriter<Formatter>(max_severity) {}

    virtual void write(const LogRecord& record) override {
        std::string str = Formatter::format(record);
        Lock lock(this->mutex_);
        setColor(record.get_severity());
        print_string(str);
        reset_text_color();
    }

private:
    void setColor(Severity severity) {
        if (is_tty()) {
            switch (severity) {
                case Fatal:
                    set_text_color(Color::White, Color::DarkRed);
                    break;
                case Error:
                    set_text_color(Color::Red);
                    break;
                case Warning:
                    set_text_color(Color::Yellow);
                    break;
                case Verbose:
                    set_text_color(Color::Green);
                    break;
                case Debug:
                    set_text_color(Color::Cyan);
                    break;
                default:
                    break;
            }
        }
    }

};
}  // namespace mel

//==============================================================================
// CLASS DOCUMENTATIOn
//==============================================================================
