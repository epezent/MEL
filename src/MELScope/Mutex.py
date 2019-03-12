# MIT License
#
# Copyright (c) 2019 Mechatronics and Haptic Interfaces Lab - Rice University
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Author(s): Evan Pezent (epezent@rice.edu)

import ctypes
from ctypes import wintypes

# Create ctypes wrapper for Win32 functions we need, with correct argument/return types
# python2 should use CreateMutexA w/ wintypes.LPCSTR
_CreateMutex = ctypes.windll.kernel32.CreateMutexW
_CreateMutex.argtypes = [wintypes.LPCVOID, wintypes.BOOL, wintypes.LPCWSTR]
_CreateMutex.restype = wintypes.HANDLE

_WaitForSingleObject = ctypes.windll.kernel32.WaitForSingleObject
_WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
_WaitForSingleObject.restype = wintypes.DWORD

_ReleaseMutex = ctypes.windll.kernel32.ReleaseMutex
_ReleaseMutex.argtypes = [wintypes.HANDLE]
_ReleaseMutex.restype = wintypes.BOOL

_CloseHandle = ctypes.windll.kernel32.CloseHandle
_CloseHandle.argtypes = [wintypes.HANDLE]
_CloseHandle.restype = wintypes.BOOL

class Mutex(object):
    """A named, system-wide mutex that can be acquired and released."""

    def __init__(self, name, acquired=False):
        """Create named mutex with given name, also acquiring mutex if acquired is True.
        Mutex names are case sensitive, and a filename (with backslashes in it) is not a
        valid mutex name. Raises WindowsError on error.

        """
        self.name = name
        self.acquired = acquired
        ret = _CreateMutex(None, False, name)
        if not ret:
            raise ctypes.WinError()
        self.handle = ret
        if acquired:
            self.try_lock()

    def try_lock(self, timeout=None):
        """Acquire ownership of the mutex, returning True if acquired. If a timeout
        is specified, it will wait a maximum of timeout seconds to acquire the mutex,
        returning True if acquired, False on timeout. Raises WindowsError on error.

        """
        if timeout is None:
            # Wait forever (INFINITE)
            timeout = 0xFFFFFFFF
        else:
            timeout = int(round(timeout * 1000))
        ret = _WaitForSingleObject(self.handle, timeout)
        if ret in (0, 0x80):
            # Note that this doesn't distinguish between normally acquired (0) and
            # acquired due to another owning process terminating without releasing (0x80)
            self.acquired = True
            return True
        elif ret == 0x102:
            # Timeout
            self.acquired = False
            return False
        else:
            # Waiting failed
            raise ctypes.WinError()

    def release(self):
        """Relase an acquired mutex. Raises WindowsError on error."""
        ret = _ReleaseMutex(self.handle)
        if not ret:
            raise ctypes.WinError()
        self.acquired = False

    def close(self):
        """Close the mutex and release the handle."""
        if self.handle is None:
            # Already closed
            return
        ret = _CloseHandle(self.handle)
        if not ret:
            raise ctypes.WinError()
        self.handle = None

    __del__ = close

    def __repr__(self):
        """Return the Python representation of this mutex."""
        return '{0}({1!r}, acquired={2})'.format(
                self.__class__.__name__, self.name, self.acquired)

    __str__ = __repr__

    # Make it a context manager so it can be used with the "with" statement
    def __enter__(self):
        self.try_lock()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.release()


#==============================================================================
# Example:
#==============================================================================
# from Mutex import Mutex

# b = Mutex("my_mutex")

# b.try_lock()
# print("B can continue now")
# b.release()
