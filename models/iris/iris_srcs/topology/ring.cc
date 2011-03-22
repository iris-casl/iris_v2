#ifndef  _ring_cc_INC
#define  _ring_cc_INC

#include        "ring.h"

Ring::Ring()
{

}

Ring::~Ring()
{
    for ( uint i=0 ; i<no_nodes; i++ )
    {
        delete terminals[i];
        delete interfaces[i];
        delete routers[i];
    }

}

void
Ring::parse_config(std::map<std::string, std::string>& p)
{
    /* Defaults */
    no_nodes = 8;
    
    ports = 3;
    vcs = 2;
    credits = 1;

    
    /* Override defaults */
    map<std::string, std::string>:: iterator it;
    it = p.find("no_nodes");
    if ( it != p.end())
        no_nodes = atoi((it->second).c_str());

    it = p.find("no_ports");
    if ( it != p.end())
        ports = atoi((it->second).c_str());

    it = p.find("credits");
    if ( it != p.end())
        credits = atoi((it->second).c_str());

    it = p.find("no_vcs");
    if ( it != p.end())
        vcs = atoi((it->second).c_str());


    return;
}

std::string
Ring::print_stats(component_type ty)     
{
    std::stringstream str;
    switch ( ty ) {
        case IRIS_TERMINAL:	
            for( uint i=0; i<no_nodes; i++)
                str << terminals[i]->print_stats();
            break;

        case IRIS_INTERFACE:	
            break;

        case IRIS_ROUTER:	
            for( uint i=0; i<routers.size(); i++)
                str << routers[i]->print_stats();
            break;

        default:	
            break;
    }				/* -----  end switch  ----- */

    return str.str();
}

void
Ring::connect_interface_terminal()
{
    /* Connect INTERFACE -> TERMINAL*/
    for( uint i=0; i<no_nodes; i++)
    {
        terminals.at(i)->ni = static_cast<manifold::kernel::Component*>(interfaces.at(i));
        interfaces.at(i)->terminal = static_cast<manifold::kernel::Component*>(terminals.at(i));
        assert ( interfaces.at(i)->terminal != NULL );
        assert ( terminals.at(i)->ni!= NULL );
    }
    return;
}

void
Ring::connect_interface_routers()
{
    int LATENCY = 1;
    /*  Connect for the output links of the router */
    for( uint i=0; i<no_nodes; i++)
    {
        /*  Interface DATA */
        manifold::kernel::Manifold::Connect(interface_ids.at(i), 0, 
                                            router_ids.at(i), 2*ports,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
        manifold::kernel::Manifold::Connect(router_ids.at(i), 0, 
                                            interface_ids.at(i), 2,
                                            &IrisInterface::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        /*  Interface SIGNAL */
        manifold::kernel::Manifold::Connect(interface_ids.at(i), 1, 
                                            router_ids.at(i), 3*ports,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
        manifold::kernel::Manifold::Connect(router_ids.at(i), ports, 
                                            interface_ids.at(i), 3,
                                            &IrisInterface::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

    }

    return;
}

void
Ring::set_router_outports(uint ind)
{
    for ( uint i=0; i<ports; i++)
    {
        routers.at(ind)->data_outports.push_back(i);
        routers.at(ind)->signal_outports.push_back(ports+i);
    }

    return;
}

void
Ring::connect_routers()
{
    int LATENCY = 1;

    // Configure east - west links for the routers.. in order first WEST then
    // EAST
    for ( uint i=1; i<no_nodes; i++)
    {

#ifdef _DBG_TOP
        int tmp4 = router_ids.at(i);
        int tmp6 = router_ids.at(i-1);
        printf ( "\n Connect %d p%d -> %d p%d", tmp4,1, tmp6, 2*ports+2);
        printf ( "\n Connect %d p%d -> %d p%d", tmp4,ports+1, tmp6, 3*ports+2);

        printf ( "\n Connect %d p%d -> %d p%d", tmp6,2, tmp4, 2*ports+1);
        printf ( "\n Connect %d p%d -> %d p%d", tmp6,ports+2, tmp4, 3*ports+1);
#endif
        // going west  <-
        /* Router->Router DATA */
        manifold::kernel::Manifold::Connect(router_ids.at(i), 1, 
                                            router_ids.at(i-1), 2*ports+2,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        /* Router->Router SIGNAL */
        manifold::kernel::Manifold::Connect(router_ids.at(i), ports+1, 
                                            router_ids.at(i-1), 3*ports+2,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        // going east  ->
        /* Router->Router DATA */
        manifold::kernel::Manifold::Connect(router_ids.at(i-1), 2, 
                                            router_ids.at(i), 2*ports+1,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        /* Router->Router SIGNAL */
        manifold::kernel::Manifold::Connect(router_ids.at(i-1), ports+2, 
                                            router_ids.at(i), 3*ports+1,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
    }

#ifdef _DBG_TOP
        int tmp4 = router_ids.at(0);
        int tmp6 = router_ids.at(no_nodes-1);
        printf ( "\n Connect %d p%d -> %d p%d", tmp4,1, tmp6, 2*ports+2);
        printf ( "\n Connect %d p%d -> %d p%d", tmp4,ports+1, tmp6, 3*ports+2);

        printf ( "\n Connect %d p%d -> %d p%d", tmp6,2, tmp4, 2*ports+1);
        printf ( "\n Connect %d p%d -> %d p%d", tmp6,ports+2, tmp4, 3*ports+1);
#endif

    // router 0 and end router
    // going west <-
    manifold::kernel::Manifold::Connect(router_ids.at(0), 1, 
                                        router_ids.at(no_nodes-1), 2*ports+2,
                                        &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

    manifold::kernel::Manifold::Connect(router_ids.at(0), ports+1, 
                                        router_ids.at(no_nodes-1), 3*ports+2,
                                        &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
    
    // going east ->
    manifold::kernel::Manifold::Connect(router_ids.at(no_nodes-1), 2, 
                                        router_ids.at(0), 2*ports+1,
                                        &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
    manifold::kernel::Manifold::Connect(router_ids.at(no_nodes-1), ports+2, 
                                        router_ids.at(0), 3*ports+1,
                                        &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));


    return;
}

void
Ring::assign_node_ids( component_type t)
{
    switch ( t)
    {
        case IRIS_TERMINAL:
            {
                for ( uint i=0; i<no_nodes; i++)
                    terminals.at(i)->node_id = i;
                break;
            }
        case IRIS_INTERFACE:
            {
                for ( uint i=0; i<no_nodes; i++)
                    interfaces.at(i)->node_id = i;
                break;
            }
        case IRIS_ROUTER:
            {
                for ( uint i=0; i<no_nodes; i++)
                    routers.at(i)->node_id = i;
                break;
            }
        default:
            {
                _sim_exit_now(" ERROR: assign_node_ids %d", t);
                break;
            }
    }
}
#endif   /* ----- #ifndef _ring_cc_INC  ----- */

