/*
 * =====================================================================================
 *
 *       Filename:  simpleRouter.h
 *
 *    Description:  This file contains the description for a simple 5 stage
 *    router component
 *    It is modelled as a component within manifold
 *
 *    BW->RC->VCA->SA->ST->LT
 *    Buffer write(BW)
 *    Route Computation (RC)
 *    Virtual Channel Allocation (VCA)
 *    Switch Allocation (SA)
 *    Switch Traversal (ST)
 *    Link Traversal (LT)
 *
 *    Model Description in cycles:
 *    ---------------------------
 *    BW and RC stages happen in the same cycle ( BW pushes the flits into the
 *    input buffer and the RC unit. )
 *    VCA: Head flits request for an output virtual channel and max(pxv) requesting
 *    input ports/vcs. On winning the grant the packet requests for SA. The
 *    winner is cleared when the tail exits.
 *    SA: Pick one output port from n requesting input ports (0<n<p) for the pxp crossbar 
 *    ST: Move the flits across the crossbar and push it out on the link
 *    LT: This is not modelled within the router and is part of the link component.
 *
 *
 *        Version:  1.0
 *        Created:  02/08/2011 10:50:32 AM
 *       Revision:  none
 *       Compiler:  g++/mpicxx
 *
 *         Author:  Mitchelle Rasquinha
 *        Company:  Georgia Institute of Technology
 *
 * =====================================================================================
 */


#ifndef  SIMPLEROUTER_H_INC
#define  SIMPLEROUTER_H_INC

#include	"../../interfaces/genericHeader.h"
#include	"../../interfaces/irisRouter.h"
#include	"../../interfaces/irisInterface.h"
#include	"../data_types/linkData.h"
#include	"genericBuffer.h"
#include	"genericSwitchArbiter.h"
#include	"genericRC.h"
#include	"genericVcAllocator.h"

//extern class Clock* master_clock;

class InputBufferState
{
    public:
        uint input_port;
        uint input_channel;
        uint output_port;
        uint output_channel;
        uint64_t pkt_arrival_time;
        uint pkt_length;
        std::vector < uint > rc_out_vcs;
        std::vector < uint > rc_out_ports;
        RouterPipeStage pipe_stage;
        message_class mclass;
        uint64_t address;
        uint src_node;
        uint destination;
        bool clear_message;
        bool sa_head_done;
        std::vector<uint> possible_oports;
        std::vector<uint> possible_ovcs;

        std::string toString() const;
#ifdef _DEBUG
        uint fid;
#endif
};

class SimpleRouter: public IrisRouter
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        SimpleRouter (); 
        ~SimpleRouter (); 

        /* ====================  Event handlers     ======================================= */
        void handle_link_arrival( int inputid, LinkData* data); 

        /* ====================  Clocked funtions  ======================================= */
        void tick (void);
        void tock (void);

        /* ================ Functions Local to this class ====================== */
        void send_credit_back( uint i);
        std::vector< std::vector<uint> > downstream_credits;
        void init( void );
        void parse_config(std::map<std::string,std::string>& p);
        string print_stats() const;

        /*! These are the statistics variables */
        uint64_t stat_packets_in;
        uint64_t stat_packets_out;
        uint64_t stat_flits_in;
        uint64_t stat_flits_out;

        uint64_t avg_router_latency;
        uint64_t stat_last_flit_out_cycle;
        vector< vector<uint64_t> > stat_pp_packets_out;
        vector< vector<uint64_t> > stat_pp_pkt_out_cy;
        vector< vector<uint64_t> > stat_pp_avg_lat;

        std::vector <InputBufferState> input_buffer_state;
    protected:

    private:
        std::vector <GenericBuffer> in_buffers;
        std::vector <GenericRC> decoders;
        GenericVcAllocator vca;
        GenericSwitchArbiter swa;

        void do_switch_traversal();
        void do_switch_allocation();
        void do_vc_allocation();
        void do_input_buffering(HeadFlit*, uint, uint);

        uint ports;
        uint vcs;
        uint credits;
        ROUTING_SCHEME rc_method;
        uint no_nodes;
        uint grid_size;

        // stats
        uint64_t stat_last_packet_out_cycle;

}; /* -----  end of class SimpleRouter  ----- */

#endif   /* ----- #ifndef SIMPLEROUTER_H_INC  ----- */
