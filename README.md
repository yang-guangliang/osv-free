# Hybrid-postMessage & OSV-Free

## Research Paper

We present the details of our work in the following research paper:

**Study and Mitigation of Origin Stripping Vulnerabilities in Hybrid-postMessage Enabled Mobile Applications**  
Guangliang Yang, Jeff Huang, Guofei Gu, Abner Mendoza.   
*In Proc. of the 39th IEEE Symposium on Security and Privacy (S&P'18), San Francisco, CA, May 2018.*



If you use *Hybrid-postMessage* or *OSV-Free* in a scientific publication, we would appreciate citations using this following Bibtex entry:

``` tex
@inproceedings{OSV_SP18,
 title = {Study and Mitigation of Origin Stripping Vulnerabilities in Hybrid-postMessage Enabled Mobile Applications},
 author = {Guangliang Yang and Jeff Huang and Guofei Gu and Abner Mendoza}, 
 year = {2018}, 
 month = {May},
 booktitle = {Proceedings of the 39th IEEE Symposium on Security and Privacy (SP'18)}
}
```

## Introduction

postMessage is popular in HTML5 based web apps to allow the communication between different origins.
 With the increasing popularity of the embedded browser (i.e., WebView) in mobile apps (i.e., hybrid apps),
postMessage has found utility in these apps.
However, different from web
apps, hybrid apps have a unique requirement that their native code (e.g.,
Java for Android) also needs to exchange messages with web code loaded in WebView.
To bridge the gap, developers typically extend postMessage by
treating the native context as a new frame, and allowing the communication
between the new frame and the web frames. We term such
extended postMessage *'hybrid postMessage'*.

However, we find that hybrid postMessage introduces new critical security
flaws: all origin information of a message is not respected or even lost during the message delivery in
hybrid postMessage.
If adversaries inject malicious code into WebView, the malicious code may
leverage the flaws to passively monitor messages that may contain
sensitive information, or actively send messages to arbitrary message
receivers and access their internal functionalities and data.
We term the novel security issue caused by hybrid postMessage
*'Origin Stripping Vulnerability'* (OSV).

To mitigate OSV from the root, we design and implement three new postMessage APIs, called
OSV-Free. Our evaluation shows that OSV-Free is secure and
fast, and it is generic and resilient to the notorious Android fragmentation
problem.

## Usage

To enable OSV-Free, the following two steps should be done:
  - the customized webview apk file (OSV-Free) must be installed in users' devices in advance.
  - the OSVWebView must be integrated in apps to replace the official default WebView.
  
## Implemenation

The customized webview apk file (OSV-Free) is done by instrumenting the chromium source code (based on the commit '19d2c5cb7b75b0c04153fbeff487ab8e8fcfa208' at June 2017). To manually compile files, please follow the instructions (https://www.chromium.org/developers/how-tos/build-instructions-android-webview).

For convenience, the intrumented source code files are shown as follows:

```
android_webview/glue/java/com/android/webview/chromium/WebViewChromium.java    
android_webview/java/org/chromium/android_webview/AwContents.java  
content/public/android/java/org/chromium/content_public/browser/WebContents.java  
content/public/android/java/org/chromium/content/browser/webcontents/WebContentsImpl.java      

content/browser/web_contents/web_contents_android.h
content/browser/web_contents/web_contents_android.cc  
content/public/browser/message_port_provider.h  
content/browser/message_port_provider.cc  

content/browser/web_contents/web_contents_android.cc
content/browser/web_contents/web_contents_android.h  
content/browser/frame_host/render_frame_host_impl.h  
content/browser/frame_host/render_frame_host_impl.cc

content/common/frame_messages.h  

content/renderer/render_frame_impl.h  
content/renderer/render_frame_impl.cc

```

## Evaluation

All API performance is shown as follows (Table IV)    
  - N->W: 250 / 112 = 2.2321428571429 ms  
  - Official API: 158 / 60 = 2.6333333333333 ms   
  - W->N: 81 / 39 = 2.0769230769231 ms

## Contacts

OSV-Free is initially developed by Guangliang Yang at the SUCCESS lab of Texas A&M University, supervised by Prof. Guofei Gu.     
For any questions, please feel free to contact    
  Guangliang Yang (YGL at TAMU dot EDU).

