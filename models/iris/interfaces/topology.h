/*
 * =====================================================================================
 *
 *! \brief Filename:  topology.h
 *
 *    Description: This class describes the abstract base class for a generic topology
 *
 *        Version:  1.0
 *        Created:  07/19/2010 10:01:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author: Sharda Murthi, smurthi3@gatech.edu
 *        Company:  Georgia Institute of Technology
 *
 * ===================================================================================== 
*/

#ifndef TOPOLOGY_H_
#define TOPOLOGY_H_

#include	"genericHeader.h"
#include	"irisTerminal.h"
#include	"irisInterface.h"
#include	"irisRouter.h"


class Topology
{
	public:

            virtual void parse_config(std::map<std::string,std::string>& p) = 0;

            virtual void connect_interface_terminal(void) = 0;
            virtual void connect_interface_routers(void) = 0;
            virtual void connect_routers(void) = 0;
            virtual void set_router_outports( uint router_no ) = 0;
            virtual void assign_node_ids ( component_type t) = 0;

            virtual std::string print_stats(component_type t) = 0;

            std::vector <IrisRouter*> routers;
            std::vector <IrisInterface*> interfaces;
            std::vector <IrisTerminal*> terminals;

            std::vector <manifold::kernel::CompId_t> router_ids;
            std::vector <manifold::kernel::CompId_t> interface_ids;
            std::vector <manifold::kernel::CompId_t> terminal_ids;
};


#endif /* TOPOLOGY_H_ */

