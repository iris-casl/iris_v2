
#include	"../../interfaces/topology.h"
#include	"../components/simpleRouter.h"
#include	"twoNode.h"
#include	"ring.h"
#include	"torus.h"

Topology* 
get_topology(topology_type ch)
{
    if ( ch == TWO_NODE)
        return new TwoNode();
    if ( ch == RING)
        return new Ring();
    if ( ch == TORUS)
        return new Torus();

    return NULL;
}

extern Topology* tp;
extern uint no_nodes;

void
dump_state_at_deadlock( void )
{
    fprintf(stderr, "\n***** DEADLOCK ****\n");
    cout << " PIPESTAGES: " << "0=INVALID, 1=EMPTY, 2=IB, 3=FULL, 4=ROUTED, 5=VCA_REQUESTED, 6=SWA_REQUESTED, 7=SW_ALLOCATED, 8=SW_TRAVERSAL, 9=REQ_OUTVC_ARB, 10=VCA_COMPLETE " << endl;
    for ( uint i=0; i<no_nodes; i++)
    {
        SimpleRouter* rr = static_cast<SimpleRouter*>(tp->routers.at(i));
        fprintf( stderr, "\n\n------ Router %d -node %d", tp->router_ids.at(i),i);
        for ( uint ii=0; ii<rr->input_buffer_state.size(); ii++)
        {
            InputBufferState ib = rr->input_buffer_state.at(ii);
//            fprintf(stderr,"\n Msg%d ip%d ic%d op%d oc%d ps%d src%d dst%d",ii,ib.input_port, 
//                    ib.input_channel, ib.output_port, ib.output_channel, ib.pipe_stage,
//                    ib.src_node, ib.destination); 
#ifdef _DEBUG
            fprintf(stderr,"\n Msg%d ip%d ic%d op%d oc%d ps%d src%d dst%d fid:%d",ii,ib.input_port,
                    ib.input_channel, ib.output_port, ib.output_channel, ib.pipe_stage,
                    ib.src_node, ib.destination, ib.fid); 
#endif


        }
    }
    fprintf(stderr,"\n\n");
    _sim_exit_now(" **** Deadlock Proc %d ",Mytid);
}
