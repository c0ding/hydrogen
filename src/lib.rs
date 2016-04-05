// Copyright 2015 Nathan Sizemore <nathanrsizemore@gmail.com>
//
// This Source Code Form is subject to the terms of the
// Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at
// http://mozilla.org/MPL/2.0/.


#[macro_use]
extern crate log;
extern crate libc;
extern crate errno;
extern crate threadpool;
extern crate simple_slab;


use std::io::Error;
use std::sync::Arc;
use std::cell::UnsafeCell;
use std::os::unix::io::{RawFd, AsRawFd};


use config::Config;

pub mod config;

mod server;


pub trait Stream : AsRawFd + Send + Sync {
    fn recv(&mut self) -> Result<Vec<Vec<u8>>, Error>;
    fn send(&mut self, buf: &[u8]) -> Result<usize, Error>;
}

pub trait Handler {
    fn on_new_connection(&mut self, fd: RawFd) -> Arc<UnsafeCell<Stream>>;
    fn on_data_received(&mut self, stream: Arc<UnsafeCell<Stream>>, buf: Vec<u8>);
    fn on_error(&mut self, err: Error);
    fn on_connection_removed(&mut self, fd: RawFd);
}


pub fn begin<T>(handler: Box<T>, cfg: Config)
    where T: Handler + Send + Sync + 'static
{
    server::begin(handler, cfg);
}
