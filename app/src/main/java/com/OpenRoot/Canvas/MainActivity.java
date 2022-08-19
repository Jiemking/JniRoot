package com.OpenRoot.Canvas;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import com.topjohnwu.superuser.ipc.RootService;
import android.widget.Toast;

public class MainActivity extends Activity {
	public static IMutual iTestService;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		RootService.bind(new Intent(this, AIDLService.class), new AIDLConnection(false));
		findViewById(R.id.go_app).setOnClickListener(
            new OnClickListener() {
                @Override
                public void onClick(View v) {
					StartGame.showFloatWindow(MainActivity.this);
                    Toast.makeText(getApplication(), "启动", Toast.LENGTH_SHORT).show();
				}
			});
        /*
         findViewById(R.id.go_app).setOnLongClickListener(new View.OnLongClickListener() {
         @Override
         public boolean onLongClick(View view) {
         System.exit(0);
         return false;                   
         }
         });
         */
    }

}
