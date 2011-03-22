#ifndef  _torus_cc_INC
#define  _torus_cc_INC

#include        "torus.h"

Torus::Torus()
{

}

Torus::~Torus()
{
    for ( uint i=0 ; i<no_nodes; i++ )
    {
        delete terminals[i];
        delete interfaces[i];
        delete routers[i];
    }

}

void
Torus::parse_config(std::map<std::string, std::string>& p)
{
    /* Defaults */
    no_nodes = 16;
    grid_size = 4;
    
    ports = 5;
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

    it = p.find("grid_size");
    if ( it != p.end())
        grid_size = atoi((it->second).c_str());

    it = p.find("credits");
    if ( it != p.end())
        credits = atoi((it->second).c_str());

    it = p.find("no_vcs");
    if ( it != p.end())
        vcs = atoi((it->second).c_str());


    return;
}

std::string
Torus::print_stats(component_type ty)     
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
Torus::connect_interface_terminal()
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
Torus::connect_interface_routers()
{
    int LATENCY = 1;
    /*  Connect for the output links of the router */
    for( uint i=0; i<no_nodes; i++)
    {
        uint iid = interface_ids.at(i);
        uint rid = router_ids.at(i);
#ifdef _DBG_TOP
        printf ( "\n Connect %d p%d -> %d p%d", iid,0, rid, 2*ports);
        printf ( "\n Connect %d p%d -> %d p%d", rid,0, iid, 2);

        printf ( "\n Connect %d p%d -> %d p%d", iid,1, rid, 3*ports);
        printf ( "\n Connect %d p%d -> %d p%d", rid,ports, iid, 3);
#endif
        // going west  <-
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
Torus::set_router_outports(uint ind)
{
    for ( uint i=0; i<ports; i++)
    {
        routers.at(ind)->data_outports.push_back(i);
        routers.at(ind)->signal_outports.push_back(ports+i);
    }

    return;
}

void
Torus::connect_routers()
{
    int LATENCY = 1;

    // Configure east - west links for the routers.. in order first WEST then
    for ( uint i=0; i<grid_size; i++)
        for ( uint j=1; j<grid_size; j++)
        {
            uint rno = router_ids.at(i*grid_size + j);
            uint rno2 = router_ids.at(i*grid_size + j-1);

#ifdef _DBG_TOP
            printf ( "\n Connect %d p%d -> %d p%d", rno,1, rno2, 2*ports+2);
            printf ( "\n Connect %d p%d -> %d p%d", rno,ports+1, rno2, 3*ports+2);

            printf ( "\n Connect %d p%d -> %d p%d", rno2,2, rno, 2*ports+1);
            printf ( "\n Connect %d p%d -> %d p%d", rno2,ports+2, rno, 3*ports+1);
#endif
            // going west  <-
            /* Router->Router DATA */
            manifold::kernel::Manifold::Connect(rno, 1, 
                                                rno2, 2*ports+2,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

            /* Router->Router SIGNAL */
            manifold::kernel::Manifold::Connect(rno, ports+1, 
                                                rno2, 3*ports+2,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

            // going east  ->
            /* Router->Router DATA */
            manifold::kernel::Manifold::Connect(rno2, 2, 
                                                rno, 2*ports+1,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

            /* Router->Router SIGNAL */
            manifold::kernel::Manifold::Connect(rno2, ports+2, 
                                                rno, 3*ports+1,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
        }

    //connect north south
    for ( uint i=1; i<grid_size; i++)
        for ( uint j=0; j<grid_size; j++)
        {
            uint rno = router_ids.at(i*grid_size + j);
            uint up_rno =router_ids.at(i*grid_size + j - grid_size);

#ifdef _DBG_TOP
            printf ( "\n Connect %d p%d -> %d p%d", rno,3, up_rno, 2*ports+4);
            printf ( "\n Connect %d p%d -> %d p%d", rno,ports+3, up_rno, 3*ports+4);

            printf ( "\n Connect %d p%d -> %d p%d", up_rno,4, rno, 2*ports+3);
            printf ( "\n Connect %d p%d -> %d p%d", up_rno,ports+4, rno, 3*ports+3);
#endif


            // going north ^
            // Router->Router DATA 
            manifold::kernel::Manifold::Connect(rno, 3, 
                                                up_rno, 2*ports+4,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

            // Router->Router SIGNAL 
            manifold::kernel::Manifold::Connect(rno, ports+3, 
                                                up_rno, 3*ports+4,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

            // going south->
            // Router->Router DATA 
            manifold::kernel::Manifold::Connect(up_rno, 4, 
                                                rno, 2*ports+3,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

            // Router->Router SIGNAL 
            manifold::kernel::Manifold::Connect(up_rno, ports+4, 
                                                rno, 3*ports+3,
                                                &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
        }

    // connect edge nodes East-west
    for ( uint i=0; i<grid_size; i++)
    {
        uint rno = router_ids.at(i*grid_size);
        uint end_rno = router_ids.at(i*grid_size+grid_size-1);

#ifdef _DBG_TOP
        printf ( "\n Connect %d p%d -> %d p%d", rno,1, end_rno, 2*ports+2);
        printf ( "\n Connect %d p%d -> %d p%d", rno,ports+1, end_rno, 3*ports+2);

        printf ( "\n Connect %d p%d -> %d p%d", end_rno,2, rno, 2*ports+1);
        printf ( "\n Connect %d p%d -> %d p%d", end_rno,ports+2, rno, 3*ports+1);
#endif

        // router 0 and end router of every row
        // going west <-
        manifold::kernel::Manifold::Connect(rno, 1, 
                                            (end_rno), 2*ports+2,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        manifold::kernel::Manifold::Connect((rno), ports+1, 
                                            (end_rno), 3*ports+2,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        // going east ->
        manifold::kernel::Manifold::Connect(end_rno, 2, 
                                            rno, 2*ports+1,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
        manifold::kernel::Manifold::Connect(end_rno, ports+2, 
                                            rno, 3*ports+1,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

    }


    // connect edge nodes north-south
    for ( uint i=0; i<grid_size; i++)
    {
        uint rno = router_ids.at(i);
        uint end_rno = router_ids.at(grid_size*(grid_size-1)+i);

#ifdef _DBG_TOP
        printf ( "\n Connect %d p%d -> %d p%d", rno,3, end_rno, 2*ports+4);
        printf ( "\n Connect %d p%d -> %d p%d", rno,ports+3, end_rno, 3*ports+4);

        printf ( "\n Connect %d p%d -> %d p%d", end_rno,4, rno, 2*ports+3);
        printf ( "\n Connect %d p%d -> %d p%d", end_rno,ports+4, rno, 3*ports+3);
#endif

        // router 0 and end router of every row
        // going west <-
        manifold::kernel::Manifold::Connect((rno), 3, 
                                            (end_rno), 2*ports+4,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        manifold::kernel::Manifold::Connect((rno), ports+3, 
                                            (end_rno), 3*ports+4,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

        // going east ->
        manifold::kernel::Manifold::Connect(end_rno, 4, 
                                            rno, 2*ports+3,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));
        manifold::kernel::Manifold::Connect(end_rno, ports+4, 
                                            rno, 3*ports+3,
                                            &IrisRouter::handle_link_arrival , static_cast<manifold::kernel::Ticks_t>(LATENCY));

    }

    return;
}

void
Torus::assign_node_ids( component_type t)
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

#endif   /* ----- #ifndef _torus_cc_INC  ----- */

