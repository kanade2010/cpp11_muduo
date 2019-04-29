// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "EventLoopThreadPool.hpp"
#include "EventLoop.hpp"
#include "EventLoopThread.hpp"
#include "Logger.hpp"

#include <stdio.h>

using namespace muduo;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg, int numThreads)
  : m_baseLoop(baseLoop),
    m_name(nameArg),
    m_started(false),
    m_numThreads(numThreads),
    m_next(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start()
{
  assert(!m_started);
  m_baseLoop->assertInLoopThread();

  m_started = true;

  for (int i = 0; i < m_numThreads; ++i)
  {
    char buf[m_name.size() + 32];
    snprintf(buf, sizeof buf, "%s%d", m_name.c_str(), i);
    EventLoopThread* t = new EventLoopThread();
    m_threads.push_back(t);
    m_loops.push_back(t->startLoop());
  }

}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  m_baseLoop->assertInLoopThread();
  assert(m_started);
  EventLoop* loop = m_baseLoop;

  if (!m_loops.empty())
  {
    // round-robin
    loop = m_loops[m_next];
    ++m_next;
    if (static_cast<size_t>(m_next) >= m_loops.size())
    {
      m_next = 0;
    }
  }
  return loop;
}

EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashCode)
{
  m_baseLoop->assertInLoopThread();
  EventLoop* loop = m_baseLoop;

  if (!m_loops.empty())
  {
    loop = m_loops[hashCode % m_loops.size()];
  }
  return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
  m_baseLoop->assertInLoopThread();
  assert(m_started);
  if (m_loops.empty())
  {
    return std::vector<EventLoop*>(1, m_baseLoop);
  }
  else
  {
    return m_loops;
  }
}
