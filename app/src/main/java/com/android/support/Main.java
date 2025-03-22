package com.android.support;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.provider.Settings;
import android.widget.Toast;


import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Log;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class Main {


    private static final String TAG = "ApkSignatureChecker";

    
        

    // Chuyển đổi mảng byte thành chuỗi hex
    private static String byteArrayToHexString(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte b : bytes) {
            sb.append(Integer.toString((b & 0xff) + 0x100, 16).substring(1));
        }
        return sb.toString();
    }

    //Load lib
    static {
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("MyLibName");
    }

    private static native void CheckOverlayPermission(Context context);

    public static void StartWithoutPermission(Context context) {
        CrashHandler.init(context, true);
        if (context instanceof Activity) {
            //Check if context is an Activity.
            Menu menu = new Menu(context);
            menu.SetWindowManagerActivity();
            menu.ShowMenu();
        } else {
            //Anything else, ask for permission
            CheckOverlayPermission(context);
        }
    }

    public static void Start(Context context) {
    
        try {
            // Lấy thông tin về ứng dụng từ context
            PackageManager pm = context.getPackageManager();
            String packageName = context.getPackageName();
            PackageInfo packageInfo = pm.getPackageInfo(packageName, PackageManager.GET_SIGNATURES);

            // Lấy chữ ký của ứng dụng
            Signature[] signatures = packageInfo.signatures;
            byte[] signatureBytes = signatures[0].toByteArray();

            // Tính toán mã băm SHA1 của chữ ký
            MessageDigest md = MessageDigest.getInstance("SHA1");
            byte[] digest = md.digest(signatureBytes);
            String signatureSHA1 = byteArrayToHexString(digest);

            // In ra chữ ký SHA1 để kiểm tra
            Log.d(TAG, "SHA1 Signature: " + signatureSHA1);

            // Kiểm tra chữ ký và cảnh báo nếu có thay đổi
            if (!signatureSHA1.equals("YOUR_EXPECTED_SIGNATURE")) {
                Log.e(TAG, "WARNING: APK signature has changed. Possible tampering!");
                // Thực hiện các hành động cảnh báo hoặc ngăn chặn ở đây
            }
        } catch (PackageManager.NameNotFoundException | NoSuchAlgorithmException e) {
            Log.e(TAG, "Error while checking APK signature: " + e.getMessage());
            e.printStackTrace();
        }
    
        CrashHandler.init(context, false);

        CheckOverlayPermission(context);
    }
}
