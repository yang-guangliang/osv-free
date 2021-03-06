// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_WEBRTC_MEDIA_STREAM_TRACK_ADAPTER_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_WEBRTC_MEDIA_STREAM_TRACK_ADAPTER_H_

#include <memory>

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/renderer/media/remote_media_stream_track_adapter.h"
#include "content/renderer/media/webrtc/media_stream_video_webrtc_sink.h"
#include "content/renderer/media/webrtc/webrtc_audio_sink.h"
#include "content/renderer/media/webrtc/webrtc_media_stream_track_adapter.h"
#include "third_party/WebKit/public/platform/WebMediaStream.h"
#include "third_party/WebKit/public/platform/WebMediaStreamTrack.h"
#include "third_party/webrtc/api/mediastreaminterface.h"

namespace content {

class PeerConnectionDependencyFactory;

// This is a mapping between a webrtc and blink media stream track. It takes
// care of creation, initialization and disposing of tracks independently of
// media streams.
// There are different sinks/adapters used whether the track is local or remote
// and whether it is an audio or video track; this adapter hides that fact and
// lets you use a single class for any type of track.
class CONTENT_EXPORT WebRtcMediaStreamTrackAdapter
    : public base::RefCountedThreadSafe<WebRtcMediaStreamTrackAdapter> {
 public:
  // Invoke on the main thread. The returned adapter is fully initialized, see
  // |is_initialized|.
  static scoped_refptr<WebRtcMediaStreamTrackAdapter> CreateLocalTrackAdapter(
      PeerConnectionDependencyFactory* factory,
      const scoped_refptr<base::SingleThreadTaskRunner>& main_thread,
      const blink::WebMediaStreamTrack& web_track);
  // Invoke on the webrtc signaling thread. Initialization finishes on the main
  // thread in a post, meaning returned adapters are ensured to be initialized
  // in posts to the main thread, see |is_initialized|.
  static scoped_refptr<WebRtcMediaStreamTrackAdapter> CreateRemoteTrackAdapter(
      PeerConnectionDependencyFactory* factory,
      const scoped_refptr<base::SingleThreadTaskRunner>& main_thread,
      webrtc::MediaStreamTrackInterface* webrtc_track);
  // Must be called before all external references are released (i.e. before
  // destruction). Invoke on the main thread. Disposing may finish
  // asynchronously using the webrtc signaling thread and the main thread. After
  // calling this method it is safe to release all external references to the
  // adapter.
  void Dispose();

  // The adapter must be initialized in order to use |web_track| and
  // |webrtc_track|.
  bool is_initialized() const;
  const blink::WebMediaStreamTrack& web_track() const;
  webrtc::MediaStreamTrackInterface* webrtc_track() const;
  bool IsEqual(const blink::WebMediaStreamTrack& web_track) const;

  // For testing.
  WebRtcAudioSink* GetLocalTrackAudioSinkForTesting() {
    return local_track_audio_sink_.get();
  }
  MediaStreamVideoWebRtcSink* GetLocalTrackVideoSinkForTesting() {
    return local_track_video_sink_.get();
  }
  RemoteAudioTrackAdapter* GetRemoteAudioTrackAdapterForTesting() {
    return remote_audio_track_adapter_.get();
  }
  RemoteVideoTrackAdapter* GetRemoteVideoTrackAdapterForTesting() {
    return remote_video_track_adapter_.get();
  }

 protected:
  friend class base::RefCountedThreadSafe<WebRtcMediaStreamTrackAdapter>;

  WebRtcMediaStreamTrackAdapter(
      PeerConnectionDependencyFactory* factory,
      const scoped_refptr<base::SingleThreadTaskRunner>& main_thread);
  virtual ~WebRtcMediaStreamTrackAdapter();

 private:
  // Initialization of local tracks occurs on the main thread.
  void InitializeLocalAudioTrack(const blink::WebMediaStreamTrack& web_track);
  void InitializeLocalVideoTrack(const blink::WebMediaStreamTrack& web_track);
  // Initialization of remote tracks starts on the webrtc signaling thread and
  // finishes on the main thread.
  void InitializeRemoteAudioTrack(
      webrtc::AudioTrackInterface* webrtc_audio_track);
  void InitializeRemoteVideoTrack(
      webrtc::VideoTrackInterface* webrtc_video_track);
  void FinalizeRemoteTrackInitializationOnMainThread();

  // Disposing starts and finishes on the main thread. Local tracks and remote
  // video tracks are disposed synchronously. Remote audio tracks are disposed
  // asynchronously with a jump to the webrtc signaling thread and back.
  void DisposeLocalAudioTrack();
  void DisposeLocalVideoTrack();
  void DisposeRemoteAudioTrack();
  void DisposeRemoteVideoTrack();
  void UnregisterRemoteAudioTrackAdapterOnSignalingThread();
  void FinalizeRemoteTrackDisposingOnMainThread();

  // Pointer to a |PeerConnectionDependencyFactory| owned by the |RenderThread|.
  // It's valid for the lifetime of |RenderThread|.
  PeerConnectionDependencyFactory* const factory_;
  scoped_refptr<base::SingleThreadTaskRunner> main_thread_;

  // This class is immutable between being initialized (triggered by
  // |Create...|) and being disposed (by calling |Dispose|). As such, no locks
  // are required in the implementation for only inherently racey actions could
  // cause problems (such as disposing while still in use).
  bool is_initialized_;
  blink::WebMediaStreamTrack web_track_;
  scoped_refptr<webrtc::MediaStreamTrackInterface> webrtc_track_;
  // If the track is local, a sink is added to the local webrtc track that is
  // owned by us.
  std::unique_ptr<WebRtcAudioSink> local_track_audio_sink_;
  std::unique_ptr<MediaStreamVideoWebRtcSink> local_track_video_sink_;
  // If the track is remote, an adapter is used that listens to notifications on
  // the remote webrtc track and notifies Blink.
  scoped_refptr<RemoteAudioTrackAdapter> remote_audio_track_adapter_;
  scoped_refptr<RemoteVideoTrackAdapter> remote_video_track_adapter_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcMediaStreamTrackAdapter);
};

}  // namespace content

#endif  // CONTENT_RENDERER_MEDIA_WEBRTC_WEBRTC_MEDIA_STREAM_TRACK_ADAPTER_H_
