// Copyright 2015 Nathan Sizemore <nathanrsizemore@gmail.com>
//
// This Source Code Form is subject to the
// terms of the Mozilla Public License, v.
// 2.0. If a copy of the MPL was not
// distributed with this file, You can
// obtain one at
// http://mozilla.org/MPL/2.0/.
//
// This Source Code Form is "Incompatible
// With Secondary Licenses", as defined by
// the Mozilla Public License, v. 2.0.


#include <stdio.h>
#include <stdlib.h>


// Because of the lack of a .h file with Rust code, we are declaring
// the extern functions in Rust land that we expect to be able to use
// once the linking phase begins
int connect(const char *address, void (*handler)(const char *));
int send_to_writer(void *w_tx, const char *buffer);


// Writer Sender<T> given to us from Rust
void *write_tx;

// Sender<T> given to us from Rust to stop the client
void *stop_tx;

// I dunno what I was going to put here, but I'll remember eventually...


// Registers the address of Rust's Sender<T> used to signal the write
// thread there is a message to send
void register_writer_tx(void *tx)
{
    write_tx = *tx;
}

// Registers the address of Rust's Sender<T> used to signal the lib
// to disconnect and exit
void register_stop_tx(void *tx)
{
    stop_tx = *tx;
}

// Calls Rust
void write(const char *buffer, int count)
{
    int result = send_to_writer(write_tx, buffer, count, stop_tx);
    if (result == -1)
    {
        // TODO - Determine if stdout msgs should report here or in Rust
    }
}