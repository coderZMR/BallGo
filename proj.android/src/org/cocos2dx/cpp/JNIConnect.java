package org.cocos2dx.cpp;

import java.io.File;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;

public class JNIConnect {

	private static Context myContext;
	  //通信的接口  activety传进来

	  public static void init(Context context) {
	          myContext=context;
	  }

	static String fun(){
		System.out.println("success!!!!");
	boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
	if (sdCardExist) { 
	     File dir = Environment.getExternalStorageDirectory();
	     String root = dir.getAbsolutePath();
	     
	     
	     
	     return root;
	}
		return "";
	}
	
	static void fun1(String s){
		
		Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
	     Uri uri = Uri.fromFile(new File(s));
	     intent.setData(uri);
	     myContext.sendBroadcast(intent);
		
	}

	
}
