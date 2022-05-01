#[link(name = "nxi", kind="static")]

extern {
   //fn register_callback(cb: extern fn(i32)) -> i32;
   //pub fn nts_data(symbol: *const libc::c_char);
   pub fn nxi_core_error(ptr: *mut libc::c_void, message: *const libc::c_char);
   pub fn nxi_page_system_open(ptr: *mut libc::c_void);
}

#[no_mangle]
pub extern "C" fn nxi_load(core: *mut libc::c_void) {
    let core = crate::nxi::Core::new(core);
    crate::nxi_load(&core);
}

pub extern "C" fn nxi_unload(_core: *mut libc::c_void) {

}
/*
pub extern "C" fn nxi_module_load(ctrader: *mut libc::c_void, cvalue: libc::c_float) {
    let trader = crate::nts::Trader::new(ctrader);
    crate::nts_main(&trader, cvalue);
}*/