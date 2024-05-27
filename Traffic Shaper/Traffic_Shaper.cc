/*
 *  File Name : Traffic_Shaper.cc
 *  Created on: 8-Mar-2022
 *  Author    : Harsha & Prajaktha
 *  Project   : Computer Networks - Task 2 - Token Bucket based Traffic Shaper.
 */


//################################################ Header Files ###########################################################//

#include <iostream>
#include <string.h>
#include <omnetpp.h>
#include <omnetpp/cqueue.h>

using namespace omnetpp;


//################################################# Macros ################################################################//

#define MAX_PACKET_ID           256

//######################################### Omnetpp Simple Module Classes #################################################//


class TransmitterNode : public cSimpleModule
{
    private:
                simtime_t       txPacketIntervalTimer;          // Timeout for transmission of data packets to traffic shaper.
                cMessage        *txPacketTimerEvent;            // Message* used for transmission timeout event.
                unsigned int    txPacketCount;                  // Used as transmit packet counter.
                cMessage        *tx_message;                    // cMessage* used for the actual data transmit message.

                unsigned int    randomBurstSizeParameter;       // Input for Exponential random number generator for creating the random burst size.
                unsigned int    randomBurstFqParameter;         // Input for Exponential random number generator for creating the random inter-arrival time.

    protected:
                virtual void initialize(void) override;
                virtual void handleMessage(cMessage* message) override;
    public:
                TransmitterNode();               // Constructors
                virtual ~TransmitterNode();      // Destructors
};


Define_Module(TransmitterNode);


//#########################################################################################################################//


class TrafficShaperNode : public cSimpleModule
{
    private:
                simtime_t       tokenGenIntervalTime;        // Timeout for generating the token in traffic shaper.
                cMessage        *tokenGenTimerEvent;         // Message* used for token generation timer timeout event.
                simtime_t       trafficCheckIntervalTime;    // Timeout for clearing the traffic in ingress queue in traffic shaper.
                cMessage        *trafficCheckTimerEvent;     // Message* used for clearing the ingress queue timer timeout event.

                cMessage        *tokenMessage;               // Message* used for token messages.
                cMessage        *ingressMessage;             // Message* used for ingress messages.
                cQueue          *tokenQueuePtr;              // handler for token buffer queue
                cQueue          *ingressQueuePtr;            // handler for ingress buffer queue
                unsigned int    availableTokenCount;         // used to store the available no. of tokens.
                unsigned int    tokenQueueSize;              // used to store the token queue size
                unsigned int    ingressQueueSize;            // used to store the ingress port queue size.
                unsigned int    tokenGenFrequency;           // used to store the frequency (in Ms) at with the tokens are generated.
                unsigned int    tokenCounter;                // used to count the generated tokens.


    protected:
                virtual void initialize(void) override;
                virtual void handleMessage(cMessage* message) override;

    public:
                TrafficShaperNode();               // Constructors
                virtual ~TrafficShaperNode();      // Destructors
};


Define_Module(TrafficShaperNode);


//#########################################################################################################################//


class ReceiverNode : public cSimpleModule
{
    private:
                   unsigned int    rxPacketCount;                  // Used as receive packet counter.
    protected:
                   virtual void initialize(void) override;
                   virtual void handleMessage(cMessage* message) override;
    public:
                   ReceiverNode();               // Constructors
                   virtual ~ReceiverNode();      // Destructors
};


Define_Module(ReceiverNode);


//##################################### Transmitter Simple Module Class Functions #########################################//


// This is Transmitter Node constructor function.
TransmitterNode::TransmitterNode()
{
    // Timeout event message pointers.
    txPacketTimerEvent = nullptr;
    tx_message         = nullptr;

    // Variable initializations
    txPacketCount       = 0;
    randomBurstSizeParameter = 0;
    randomBurstFqParameter = 0;

}

// This is Transmitter Node Destructor function.
TransmitterNode::~TransmitterNode()
{
    // Free up the heap by deleting the dynamically allocated memory.
    cancelAndDelete(txPacketTimerEvent);
    delete tx_message;

}


// Function to initialize the transmitter.
void TransmitterNode::initialize(void)
{
    double random_time;
    int random;

    // Copy the values from the .ini file.
    randomBurstSizeParameter  = par("RandomBurstSize");
    randomBurstFqParameter  = par("RandomBurstFrequency");

    EV << "Input Parameters For Transmitter Node :\n";
    EV << "Random Burst Size Input Parameter = " << randomBurstSizeParameter << "\n";
    EV << "Random Burst Time Interval Parameter = " << randomBurstFqParameter << "\n\n";

    // Initialize the timeout events.
    txPacketIntervalTimer = (2000);   // Start sending messages after 2 seconds.
    txPacketTimerEvent  = new cMessage("TX!");

    // Scheduling the Timers.
    scheduleAt(simTime()+txPacketIntervalTimer,txPacketTimerEvent);

    EV << "Transmitter : Initialization Done!\n";
}



// Function to handle the event messages in the transmitter.
void TransmitterNode::handleMessage(cMessage* message)
{
    // Check if is is a transmitter sending cycle timeout event message.
    if(message == txPacketTimerEvent)
    {
        double random_number;
        unsigned int random_time;
        char messageName[20];

        random_number = exponential((double)randomBurstSizeParameter, 0);
        random_time = (unsigned int) random_number;

        if (randomBurstSizeParameter == 0)
        {
            sprintf(messageName,"D:%d",txPacketCount);
            tx_message = new cMessage(messageName);
            send(tx_message,"egress_port");

            txPacketCount ++;

            if(txPacketCount >= MAX_PACKET_ID)
            {
                txPacketCount = 0;
            }
        }
        else
        {
            if (random_time != 0)
            {
                for (int idx = 0;idx < random_time;idx++)
                {
                    // Send the next message to the traffic shaper node.
                    sprintf(messageName,"D:%d",txPacketCount);
                    tx_message = new cMessage(messageName);
                    send(tx_message,"egress_port");

                    txPacketCount ++;

                    if(txPacketCount >= MAX_PACKET_ID)
                    {
                        txPacketCount = 0;
                    }
                }
            }
        }


        // Random Interval Delay Creation
        random_number = exponential(randomBurstFqParameter, 0);
        random_time = (unsigned int) random_number;

        // If random time is zero then maintain a minimum gap of 10ms.
        if (random_time == 0)
        {
            random_time = 10;
        }

        txPacketIntervalTimer = (simtime_t)random_time;

        // Reschedule the timer event again.
        scheduleAt(simTime()+txPacketIntervalTimer,txPacketTimerEvent);
    }
    // It's a message from the receiver which is not expected here.
    else
    {
        EV << "Transmitter : Unexpected Message!\n";
        bubble("Unexpected Message!");
        delete message;
    }
}

//################################## Traffic Shaper Simple Module Class Functions #########################################//


// This is Traffic shaper Node constructor function.
TrafficShaperNode::TrafficShaperNode()
{
    // Timeout event message pointers.
    tokenGenTimerEvent     = nullptr;
    trafficCheckTimerEvent = nullptr;

    tokenMessage = nullptr;
    tokenQueuePtr = nullptr;
    ingressQueuePtr = nullptr;

    // Variable initializations
    availableTokenCount = 0;
    tokenQueueSize      = 0;
    ingressQueueSize    = 0;
    tokenGenFrequency   = 0;
    tokenCounter        = 0;


}

// This is Traffic shaper Node Destructor function.
TrafficShaperNode::~TrafficShaperNode()
{
    // Free up the heap by deleting the dynamically allocated memory.
    cancelAndDelete(tokenGenTimerEvent);
    cancelAndDelete(trafficCheckTimerEvent);

    delete tokenMessage;
    delete tokenQueuePtr;
    delete ingressQueuePtr;

}


// Function to initialize the Traffic shaper.
void TrafficShaperNode::initialize(void)
{
    static cPacketQueue::Comparator *cmpPtr = nullptr;
    static cQueue  TokenQueue("Token-Queue",cmpPtr);
    static cQueue  IngressQueue("Ingress-Queue",cmpPtr);

    tokenQueuePtr = &TokenQueue;
    ingressQueuePtr = &IngressQueue;

    // Copy the values from the .ini file.
    ingressQueueSize  = par("IngressQueueSize");
    tokenQueueSize    = par("TokenQueueSize");
    tokenGenFrequency = par("TokenGenRateMs");
    trafficCheckIntervalTime = par("TokenCheckDispatchRateMS");

    EV << "Input Parameters For Traffic Shaper Node :\n";
    EV << "Ingress Buffer Queue Size = " << ingressQueueSize << "\n";
    EV << "'Token' Buffer Queue Size = " << tokenQueueSize << "\n";
    EV << "Token Generation Interval = " << tokenGenFrequency << " Ms \n";
    EV << "Token Check&Disp Interval = " << trafficCheckIntervalTime << " Ms \n\n";


    // Initialize the timeout events.
    tokenGenIntervalTime = tokenGenFrequency;
    tokenGenTimerEvent  = new cMessage("TOK!");

    trafficCheckTimerEvent = new cMessage("TRF!");

    // Scheduling the Timers.
    scheduleAt(simTime()+tokenGenIntervalTime,tokenGenTimerEvent);
    scheduleAt(simTime()+trafficCheckIntervalTime,trafficCheckTimerEvent);

    EV << "Traffic Shaper Node : Initialization Done!\n";
}



// Function to handle the event messages in the Traffic shaper.
void TrafficShaperNode::handleMessage(cMessage* message)
{
    // Check if is is a transmitter sending cycle timeout event message.
    if(message == tokenGenTimerEvent)
    {
        //Check if the token buffer queue capacity is not full.
        if (tokenQueuePtr->getLength() < tokenQueueSize)
        {
            char messageName[20];
            sprintf(messageName,"T:%d",tokenCounter);

            tokenMessage = new cMessage(messageName);
            tokenQueuePtr->insert(tokenMessage);
        }

        // Update the available token count.
        availableTokenCount = tokenQueuePtr->getLength();

        // Reschedule the timer event again.
        scheduleAt(simTime()+tokenGenIntervalTime,tokenGenTimerEvent);
    }
    else if(message == trafficCheckTimerEvent)
    {
         while((!tokenQueuePtr->isEmpty()) && (!ingressQueuePtr->isEmpty()))
         {
             tokenMessage = (cMessage *)tokenQueuePtr->pop();
             delete tokenMessage;

             ingressMessage = (cMessage *)ingressQueuePtr->pop();
             send(ingressMessage,"egress_port");

         }

         // Reschedule the timer event again.
         scheduleAt(simTime()+tokenGenIntervalTime,trafficCheckTimerEvent);
    }
    // It's a message from the transmitter which is expected here.
    else
    {
        EV << "Traffic Node: Received Message ID -> " << message->getName() <<"\n";

        //Check if the token buffer queue capacity is not full.
        if (ingressQueuePtr->getLength() < ingressQueueSize)
        {
            bubble("RX OK!");
            ingressQueuePtr->insert(message);

            EV << "Ingress Queue Length: " << ingressQueuePtr->getLength() << "\n" ;
        }
        else
        {
            // No space in the ingress buffer queue. Delete it .
            bubble("Ingress Buffer Overflow!");
            EV << "Ingress Buffer Overflow!!! " << "Discarded Message ID: " << message->getName() << "\n";
            delete message;
        }
    }
}


//####################################### Receiver Simple Module Class Functions #########################################//


// This is Receiver Node constructor function.
ReceiverNode::ReceiverNode()
{

}

// This is Receiver Node Destructor function.
ReceiverNode::~ReceiverNode()
{

}


// Function to initialize the Receiver.
void ReceiverNode::initialize(void)
{
    rxPacketCount = 0;
    EV << "Receiver : Initialization Done!\n";
}



// Function to handle the event messages in the Receiver.
void ReceiverNode::handleMessage(cMessage* message)
{
    // It's a message from the transmitter via traffic shaper.
    EV << "Receiver : Total Messages = " << rxPacketCount << "   Latest Message ID is " << message->getName() << "\n";
    rxPacketCount ++;
    bubble("RX OK!");
    delete message;
}


//#########################################################################################################################//
