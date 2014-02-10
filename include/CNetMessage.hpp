#ifndef CNETMESSAGE_HPP
#define CNETMESSAGE_HPP

#include "inetmessage.h"

class CNetMessage : public INetMessage
{
public:
    CNetMessage()
    {
        m_bReliable = true;
        m_NetChannel = NULL;
    }

    virtual ~CNetMessage() {};

    virtual void SetNetChannel(INetChannel * netchan) {};
    virtual void SetReliable(bool state) {};

    virtual bool Process() { return false; }

    virtual bool ReadFromBuffer(bf_read &buffer) { return false; }
    virtual bool WriteToBuffer(bf_write &buffer) { return false; }

    virtual bool IsReliable() const { return false; }

    virtual int GetType() const { return 0; }
    virtual int GetGroup() const { return 0; }
    virtual const char *GetName() const { return nullptr; }

    virtual INetChannel *GetNetChannel() const { return nullptr; }
    virtual const char *ToString() const { return nullptr; }

protected:
    bool m_bReliable;
    INetChannel* m_NetChannel;
};

#endif
