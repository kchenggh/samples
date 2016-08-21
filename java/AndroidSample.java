//  --------------------------------------------------------------
//  Copyright(C) 2016 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------
//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;

import javax.net.ssl.HttpsURLConnection;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.http.client.ClientProtocolException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import android.widget.MediaController;
import android.widget.Toast;
import android.widget.VideoView;

public class myView extends Activity
{	
	public String sErrExceedQuota;
	public int iResult = -1;
	
	private class sampleTask extends AsyncTask<myParam, Integer, Long> {
		protected Long doInBackground(myParam... params) {
			parsedMyParam = params[0];
			String url = "https://your_api_url";
			 
			try {
				URL obj = new URL(url);
				HttpsURLConnection con = (HttpsURLConnection) obj.openConnection();
				
				con.setRequestMethod("POST");
				con.setRequestProperty("User-Agent", "myapp-android");
				con.setRequestProperty("Accept-Language", "en-US,en");
				String urlParameters = "client_version=1&userID=user";
				urlParameters = urlParameters + "&appID=id&appKey=key";
				con.setDoOutput(true);
				DataOutputStream wr = new DataOutputStream(con.getOutputStream());
				wr.writeBytes(urlParameters);
				wr.flush();
				wr.close();  
				int responseCode = con.getResponseCode();
				BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
				String inputLine;
				StringBuffer response = new StringBuffer();   
				while ((inputLine = in.readLine()) != null) {
					response.append(inputLine);
				}
				in.close();
				con.disconnect();						
			} catch (ClientProtocolException e) {
			} catch (IOException e) {
				String err = e.getMessage();
			} catch(Exception e){
				String err = e.getMessage();
			}
		
			return 0L;
		}
	}
	
}