package com.OpenRoot.Canvas;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.Style;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.os.RemoteException;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ESPView extends SurfaceView implements SurfaceHolder.Callback {
	
    public ESPView(Context context) {
        super(context);
		getHolder().addCallback(this);
        setZOrderOnTop(true);
		setZOrderMediaOverlay(true);
        setBackgroundColor(Color.TRANSPARENT);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        requestFocus();
        requestFocusFromTouch();
    }

    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        Log.e("DrawView","surfaceCreated");
        try {
            MainActivity.iTestService.setSurface(holder.getSurface());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {}
	

}

