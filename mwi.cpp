#include <wayfire/plugin.hpp>
#include <wayfire/view.hpp>
#include <wayfire/core.hpp>
#include <wayfire/workspace-manager.hpp>
#include <wayfire/signal-definitions.hpp>

class mithos_wayfire_integration : public wf::plugin_interface_t
{
	std::unique_ptr<wf::workspace_manager::anchored_area> anchored_area_top;
	std::unique_ptr<wf::workspace_manager::anchored_area> anchored_area_left;
	
	wf::signal_connection_t created_cb = [=] (wf::signal_data_t *data){
			auto view = get_signaled_view(data);
			
			if (view->get_app_id() == "eu.mithos.Jupiter"){				
				int width = output->get_layout_geometry().width;
				int height = output->get_layout_geometry().height;
				
				if(view->get_title() == "eu.mithos.Jupiter-panelWindow"){
					view->move(0, 0);
					view->set_decoration(NULL);
					view->resize(width, 30);
					view->set_sticky(true);
					view->set_role(wf::VIEW_ROLE_DESKTOP_ENVIRONMENT);
					output->workspace->add_view(view, wf::LAYER_TOP);
					
					anchored_area_top = std::make_unique<wf::workspace_manager::anchored_area>();
					anchored_area_top->reflowed = 0;
					output->workspace->add_reserved_area(anchored_area_top.get());
					anchored_area_top->edge = wf::workspace_manager::ANCHORED_EDGE_TOP;
					anchored_area_top->reserved_size = 30;
					anchored_area_top->real_size = 300;
					
					output->workspace->reflow_reserved_areas();
					view->move(0, 0);
				}
				
				if(view->get_title() == "eu.mithos.Jupiter-dockWindow"){
					view->move(0, 30);
					view->set_decoration(NULL);
					view->resize(5, height-30);
					view->set_sticky(true);
					view->set_role(wf::VIEW_ROLE_DESKTOP_ENVIRONMENT);
					output->workspace->add_view(view, wf::LAYER_TOP);
					
					anchored_area_left = std::make_unique<wf::workspace_manager::anchored_area>();
					anchored_area_left->reflowed = 0;
					output->workspace->add_reserved_area(anchored_area_left.get());
					anchored_area_left->edge = wf::workspace_manager::ANCHORED_EDGE_LEFT;
					anchored_area_left->reserved_size = 5;
					anchored_area_left->real_size = 100;
					
					output->workspace->reflow_reserved_areas();
					view->move(0, 30);
				}
				
				if(view->get_title() == "eu.mithos.Jupiter-menuWindow"){
					LOGI("FOUND");
					view->move(70, 30);
					view->set_decoration(NULL);
					view->resize(400, height);
					view->set_role(wf::VIEW_ROLE_DESKTOP_ENVIRONMENT);
				}
			}
	};
	
	wf::signal_connection_t closed_cb = [=] (wf::signal_data_t *data){
			auto view = get_signaled_view(data);
			
			if (view->get_app_id() == "eu.mithos.Jupiter" && view->get_title() == "eu.mithos.Jupiter-panelWindow"){
				
				if(anchored_area_top){
					output->workspace->remove_reserved_area(anchored_area_top.get());
					anchored_area_top = nullptr;
				}
				
				if(anchored_area_left){
					output->workspace->remove_reserved_area(anchored_area_left.get());
					anchored_area_left = nullptr;
				}
				
				output->workspace->reflow_reserved_areas();
			}
	};
	
  public:
    void init() override
    {
        LOGI("Init Test");
        output->connect_signal("view-mapped", &created_cb);
        output->connect_signal("view-unmapped", &closed_cb);
    }

    void fini() override
    {
        /* Destroy plugin */
    }
};

DECLARE_WAYFIRE_PLUGIN(mithos_wayfire_integration)
