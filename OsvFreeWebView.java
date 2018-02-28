package OsvFree;

import android.content.Context;
import android.net.Uri;
import android.util.AttributeSet;
import android.webkit.WebView;

import java.lang.reflect.Method;

public class OsvFreeWebView extends WebView {
        private Object chromiumObject;
        private void init() {
                chromiumObject = null;
        }

        public OsvFreeWebView(Context context) {
                super(context);
                init();
        }

        public OsvFreeWebView(Context context, AttributeSet attrs) {
                super(context, attrs);
                init();
        }

        public OsvFreeWebView(Context context, AttributeSet attrs, int defStyleAttr) {
                super(context, attrs, defStyleAttr);
                init();
        }

        public OsvFreeWebView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
                super(context, attrs, defStyleAttr, defStyleRes);
                init();
        }

        public void initOsv() throws Exception {
                Method getWebViewProvider = WebView.class.getDeclaredMethod("getWebViewProvider");
                getWebViewProvider.setAccessible(true);
                Object provider = getWebViewProvider.invoke(this, new Object[]{});

                Method getViewDelegate = provider.getClass().getDeclaredMethod("getViewDelegate");
                getViewDelegate.setAccessible(true);
                chromiumObject = getViewDelegate.invoke(provider, new Object[]{});
        }

        public void postMessageToMainFrame(final String message, final Uri targetOrigin) throws Exception {
                Method postMessageToMainFrame = chromiumObject.getClass().getMethod("postMessageToMainFrame", new Class[]{String.class, Uri.class});
                postMessageToMainFrame.invoke(chromiumObject, new Object[]{message, targetOrigin});
        }

        public void receiveMessageFromMainFrame(final Object callback) throws Exception {
                System.out.println("@receiveMessageFromMainFrame");
                Method receiveMessageFromMainFrame = chromiumObject.getClass().getMethod("receiveMessageFromMainFrame", new Class[]{Object.class});
                receiveMessageFromMainFrame.invoke(chromiumObject, new Object[]{callback});
        }

}
