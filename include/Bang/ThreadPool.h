#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Bang/BangDefines.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/Thread.h"

NAMESPACE_BANG_BEGIN

FORWARD class Thread;
FORWARD class ThreadRunnable;

class ThreadPool
{
public:
    ThreadPool();
    virtual ~ThreadPool();

    bool TryStart(ThreadRunnable *runnable);

    void SetName(const String &name);
    void SetMaxThreadCount(int maxThreadCount);

    const String &GetName() const;
    int GetMaxThreadCount() const;

private:
    String m_threadsName = "BangPooledThread";
    List<Thread*> m_threadList;

    int m_maxThreadCount = 32;
};

NAMESPACE_BANG_END

#endif // THREADPOOL_H
