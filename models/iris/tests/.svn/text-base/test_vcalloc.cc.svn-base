/*
 * =====================================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/2011 05:57:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mitchelle Rasquinha
 *        Company:  Georgia Institute of Technology
 *
 * =====================================================================================
 */

#include "iris_srcs/components/fcfs_vca.h"
#include	<stdio.h>
#include	<iostream>
#include	<cstdlib>

int
main()
{
    GenericVcAllocator vca = *new GenericVcAllocator();
    uint ports = 3;
    uint vcs =4;
    vca.node_ip = 0;
    vca.setup ( ports,vcs);

//    for ( uint aa=0; aa<3; aa++)
//        for ( uint ab=0; ab<4; ab++)
//            vca.request(2,0,aa,ab);
            vca.request(2,0,0,0);
            vca.request(2,0,0,1);
            vca.request(2,0,0,2);
            vca.request(2,0,0,3);
            vca.request(2,1,1,2);

    for ( uint kk=0; kk<5;kk++)
    {
        printf("\n\n----------Iter %d", kk);
        vca.pick_winner();

        printf("\n port 2 winners:");
        for ( uint ai=0; ai<vca.current_winners[2].size(); ai++)
        {
            VCA_unit winner = vca.current_winners[2][ai];
            uint ip = winner.in_port;
            uint ic = winner.in_vc;
            uint op = winner.out_port;
            uint oc = winner.out_vc;
            printf(" ip%d|%d/ op%d|%d ",ip,ic,op,oc);
        }

        for ( uint ai=0; ai<vca.current_winners[2].size(); ai++)
        {
            VCA_unit winner = vca.current_winners[2][ai];
            uint ip = winner.in_port;
            uint ic = winner.in_vc;
            uint op = winner.out_port;
            uint oc = winner.out_vc;
            if ( op == 2 && oc == 0)
                vca.clear_winner(op, oc, ip, ic);
        }
    }

    exit(0);

}
