//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//


simple TransmitterNode
{
    parameters:
        @display("i=block/routing,cyan");
        int RandomBurstSize @prompt("Specify Random Burst Size Parameter") = default(2);
        int RandomBurstFrequency @prompt("Specify Random Burst Frequency Parameter") = default(200);
        
    gates:
        //input  ingress_port;
        output egress_port;      
}



simple TrafficShaperNode
{
    parameters:
        @display("i=block/routing,gold");
        int IngressQueueSize @prompt("Specify Ingress Buffer Queue Size") = default(100);
        int TokenQueueSize @prompt("Specify Token Queue Size")  = default(20);
        int TokenGenRateMs @prompt("Specify Token Generation Rate In Milliseconds")  = default(250);
        int TokenCheckDispatchRateMS @prompt("Specify Token check Rate For Dispatch In Milliseconds")  = default(100);	
        
    gates:
        input  ingress_port;
        output egress_port;
}
      
      
simple ReceiverNode
{
    parameters:
        @display("i=block/routing,cyan");
        
    gates:
        input  ingress_port;
        //output egress_port;
}      

network MyNetwork
{
    parameters:
    @display("bgb=800,400");
    
    submodules:
        TxNode : TransmitterNode
        {
            @display("p=50,200");
        }
        
        TSNode : TrafficShaperNode
        {
            @display("p=400,200");
        }
        
        RxNode : ReceiverNode
        {
            @display("p=730,200");
        }
        
    connections:
        TxNode.egress_port --> { delay = 50ms; } --> TSNode.ingress_port;
        //TSNode.egress_port --> { delay = 50ms; } --> TxNode.ingress_port;
        
        TSNode.egress_port --> { delay = 50ms; } --> RxNode.ingress_port;
        //RxNode.egress_port --> { delay = 50ms; } --> TSNode.ingress_port;
}
            