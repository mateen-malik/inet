//
// Copyright (C) 2008 Juan-Carlos Maureira
// Copyright (C) INRIA
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

package inet.applications.dhcp;

import inet.applications.contract.IApp;

//
// Implements the DHCP server protocol. DHCP (Dynamic Host Configuration Protocol),
// described in RFC 2131, provides configuration parameters to Internet hosts.
// Requires UDP.
//
// @see ~DhcpClient, ~DhcpMessage
//
simple DhcpServer like IApp
{
    parameters:
        string interfaceTableModule; // the path to the InterfaceTable module
        string interface = default(""); // interface to listen on; can be left empty if there is only one non-loopback interface
        int numReservedAddresses; // number of addresses to skip at the start of the network's address range
        int maxNumClients; // maximum number of clients (IPs) allowed to be leased
        string gateway = default(""); // gateway to assign, if empty use the interface address
        int leaseTime @unit(s); // lease time in seconds
        double startTime @unit(s) = default(0s); // application start time
        @display("i=block/app2");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
    gates:
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}

