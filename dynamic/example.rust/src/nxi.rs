pub mod capi;

use crate::nxi::capi as api;

pub struct Core
{
    core_ptr: *mut libc::c_void
}

impl Core {
    pub fn new(core_ptr: *mut libc::c_void) -> Core {
        Core{ core_ptr }
    }

    pub fn error(&self, message: &str) {
        unsafe {
            api::nxi_core_error(self.core_ptr, message.as_ptr() as *const libc::c_char);
        }
    }
}

pub struct PageSystem
{
    page_system_ptr: *mut libc::c_void
}

impl PageSystem {
    pub fn new(page_system_ptr: *mut libc::c_void) -> PageSystem {
        PageSystem{ page_system_ptr }
    }

    pub fn open(&self) {
        unsafe {
            api::nxi_page_system_open(self.page_system_ptr);
        }
    }
}