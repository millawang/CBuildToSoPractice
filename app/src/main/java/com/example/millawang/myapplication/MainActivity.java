package com.example.millawang.myapplication;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;


public class MainActivity extends Activity implements View.OnClickListener {

    static{
        System.loadLibrary("TestJni");
        System.loadLibrary("ZipPass");
    }
    public native String StringFromJNI(String id);
    public native String getZipPass(String id);

    //line post
    static final int REQUEST_ACTION_PICK = 1;
    private final String PACKAGE_NAME = "jp.naver.line.android";
    private final String CLASS_NAME = "jp.naver.line.android.activity.selectchat.SelectChatActivity";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        TextView tv = (TextView)findViewById(R.id.textv);
//        tv.setText("Millatest JNI " + getZipPass("0101343685368356"));

        Button line_post = (Button)findViewById(R.id.line_post);
        line_post.setOnClickListener(this);

        Button line_img_post = (Button)findViewById(R.id.line_img_post);
        line_img_post.setOnClickListener(this);


    }

    @Override
    public void onClick(final View v) {
        switch (v.getId()){
            case R.id.line_post:
                postToLine("test line post by android.");
                break;
            case R.id.line_img_post:
                galleryHandler();
                break;
        }
    }


    private void postToLine(String content){
        if(checkLineInstalled()) {
            Intent intent = new Intent(Intent.ACTION_SEND);
            intent.setClassName(PACKAGE_NAME, CLASS_NAME);
            intent.setType("text/plain");
            intent.putExtra(Intent.EXTRA_TEXT, content);
            startActivity(intent);
        } else {
            Toast.makeText(MainActivity.this, "Line isn't exist!", Toast.LENGTH_SHORT).show();
        }
    }

    private boolean checkLineInstalled(){
        PackageManager pm = getPackageManager();
        List<ApplicationInfo> m_appList = pm.getInstalledApplications(0);
        boolean lineInstallFlag = false;
        for (ApplicationInfo ai : m_appList) {
            if(ai.packageName.equals(PACKAGE_NAME)){
                lineInstallFlag = true;
                break;
            }
        }
        return lineInstallFlag;
    }

    public void galleryHandler() {
        if(checkLineInstalled()){
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("image/*");
            startActivityForResult(Intent.createChooser(intent,"select"), REQUEST_ACTION_PICK);
        }else{
            Toast toast = Toast.makeText(this, "LINEがインストールされていません", Toast.LENGTH_SHORT);
            toast.show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String fname = "send_image.jpg";
        String fileFullPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + File.separator + fname;
        FileOutputStream out = null;

        if(resultCode == RESULT_OK){
            if(requestCode == REQUEST_ACTION_PICK){
                Log.d("","line post onActivityResult...");
                try {
                    InputStream iStream = getContentResolver().openInputStream(data.getData());
                    ByteArrayOutputStream os = new ByteArrayOutputStream();
                    Bitmap bm = BitmapFactory.decodeStream(iStream);
                    bm.compress(Bitmap.CompressFormat.JPEG, 100, os);
                    os.flush();
                    byte[] w = os.toByteArray();
                    os.close();
                    iStream.close();
                    out = new FileOutputStream(fileFullPath);
                    out.write(w, 0, w.length);
                    out.flush();

                    Uri uri = Uri.fromFile(new File(fileFullPath));

                    Intent intent = new Intent(Intent.ACTION_SEND);

                    intent.setClassName(PACKAGE_NAME, CLASS_NAME);
                   // intent.setType("image/jpeg");
                    intent.putExtra(Intent.EXTRA_STREAM, uri);
                    intent.putExtra(Intent.EXTRA_TEXT, "android post");

                    intent.setType("text/html");

                    //Bitmapで普通に利用ができます。
                    //((ImageView)findViewById(R.id.imageView1)).setImageBitmap(bm);
                    startActivity(intent);
                }catch (IOException e) {
                    Log.d("test_error",e.getMessage());
                }
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }
}
