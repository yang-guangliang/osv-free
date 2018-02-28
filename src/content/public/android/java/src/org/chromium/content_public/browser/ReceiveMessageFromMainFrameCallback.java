// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.content_public.browser;

/**
  * An interface that allows the embedder to be notified when the results of
  * extractSmartClipData are available.
  */
/* OSV_FREE */
public interface ReceiveMessageFromMainFrameCallback {
        public void onMessageFromMainFrameReceived(String frameOrigin, String scriptOrigin, String data, boolean isProxyInvolved) throws Exception;
}
