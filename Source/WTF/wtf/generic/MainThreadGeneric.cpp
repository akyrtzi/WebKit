/*
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Justin Haygood (jhaygood@reaktix.com)
 * Copyright (C) 2016 Konstantin Tokavev <annulen@yandex.ru>
 * Copyright (C) 2016 Yusuke Suzuki <utatane.tea@gmail.com>
 * Copyright (C) 2019 Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#if !OS(LINUX)
#include <pthread.h>
#endif
#if HAVE(PTHREAD_NP_H)
#include <pthread_np.h>
#endif
#if OS(LINUX)
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <wtf/RunLoop.h>

namespace WTF {

#if !HAVE(PTHREAD_MAIN_NP) && !OS(LINUX)
static pthread_t mainThread;
#endif

void initializeMainThreadPlatform()
{
#if !HAVE(PTHREAD_MAIN_NP) && !OS(LINUX)
    mainThread = pthread_self();
#endif
}

bool isMainThread()
{
#if HAVE(PTHREAD_MAIN_NP)
    int mainThreadNp = pthread_main_np();
    ASSERT(mainThreadNp != -1);
    return mainThreadNp == 1;
#elif OS(LINUX)
    return getpid() == static_cast<pid_t>(syscall(SYS_gettid));
#else
    return pthread_equal(pthread_self(), mainThread);
#endif
}

} // namespace WTF
