mod nxi;

fn nxi_load(core: &nxi::Core)
{
    core.error("Hi from Rust !");
    //core.session().command_system().add("mod_rust", "main", |params: nxi::values| { core.page_system().open<nxi:web_page>("path") };)
}

fn nxi_unload(core: &nxi::Core)
{

}