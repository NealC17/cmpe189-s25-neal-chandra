/*
 * Copyright (c) 2011 Yufei Cheng
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  https://resilinets.org/
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#ifndef DSR_GRATUITOUS_REPLY_TABLE_H
#define DSR_GRATUITOUS_REPLY_TABLE_H

#include "ns3/callback.h"
#include "ns3/ipv4-address.h"
#include "ns3/simulator.h"
#include "ns3/timer.h"

#include <vector>

namespace ns3
{
namespace dsr
{
/**
 * The gratuitous table entries, it maintains the already sent gratuitous route reply entries.
 * When the node "promiscuously" received a packet destined for other nodes, and inferred a shorter
 * route for the data packet, it will construct a route reply and send back to the source
 */
struct GraReplyEntry
{
    Ipv4Address m_replyTo;   ///< reply to address
    Ipv4Address m_hearFrom;  ///< heard from address
    Time m_gratReplyHoldoff; ///< gratuitous reply holdoff time

    /**
     * Constructor
     *
     * \param t IPv4 address to reply to
     * \param f IPv4 address to hear from
     * \param h gratuitous hold off time
     */
    GraReplyEntry(Ipv4Address t, Ipv4Address f, Time h)
        : m_replyTo(t),
          m_hearFrom(f),
          m_gratReplyHoldoff(h)
    {
    }
};

/**
 * \ingroup dsr
 * \brief maintain the gratuitous reply
 */
class DsrGraReply : public Object
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();

    DsrGraReply();
    ~DsrGraReply() override;

    /// Set the gratuitous reply table size
    /// \param g The gratuitous reply table size
    void SetGraTableSize(uint32_t g)
    {
        GraReplyTableSize = g;
    }

    /// Get the gratuitous reply table size
    /// \returns The gratuitous reply table size
    uint32_t GetGraTableSize() const
    {
        return GraReplyTableSize;
    }

    /// Add a new gratuitous reply entry
    /// \param graTableEntry The gratuitous reply entry
    /// \return true on success
    bool AddEntry(GraReplyEntry& graTableEntry);
    /// Update the route entry if found
    /// \param replyTo Entry directed to
    /// \param replyFrom Entry heard from
    /// \param gratReplyHoldoff New gratuitous reply holdoff time
    /// \return true on success
    bool FindAndUpdate(Ipv4Address replyTo, Ipv4Address replyFrom, Time gratReplyHoldoff);
    /// Remove all expired entries
    void Purge();

    /// Remove all entries
    void Clear()
    {
        m_graReply.clear();
    }

  private:
    /// Vector of entries
    std::vector<GraReplyEntry> m_graReply;
    /// The max # of gratuitous reply entries to hold
    uint32_t GraReplyTableSize;

    /// Check if the entry is expired or not
    struct IsExpired
    {
        /**
         * Check if the entry is expired
         *
         * \param b GraReplyEntry entry
         * \return true if expired, false otherwise
         */
        bool operator()(const GraReplyEntry& b) const
        {
            return (b.m_gratReplyHoldoff < Simulator::Now());
        }
    };
};
} // namespace dsr
} // namespace ns3

#endif /* DSR_GRATUITOUS_REPLY_TABLE_H */
