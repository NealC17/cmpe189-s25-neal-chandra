#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main (int argc, char *argv[])
{
  // Default values (can be overridden by command line)
  std::string dataRate = "5Mbps";
  uint32_t packetSize = 1024;

  // Create a command line object
  CommandLine cmd;
  cmd.AddValue ("dataRate", "The data rate for the channel", dataRate);
  cmd.AddValue ("packetSize", "The packet size for the hello message", packetSize);
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue (dataRate));  // Use the command-line value
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("4ms"));      // Set delay to 4ms

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (10.0));
  serverApps.Stop (Seconds (100.0));  // Make sure the stop time is sufficient

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (packetSize)); // Use the command-line value

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (20.0));
  clientApps.Stop (Seconds (100.0)); // Make sure the stop time is sufficient

  Simulator::Stop (Seconds (100.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

