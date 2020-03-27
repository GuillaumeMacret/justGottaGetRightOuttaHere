package com.example.justgottagetrightouttahere_client.Fragments;

import android.media.Image;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.fragment.app.Fragment;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.Views.GameView;
import com.example.justgottagetrightouttahere_client.model.GameModel;

import java.text.DecimalFormat;

public class GameboardFragment extends Fragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle bundle){
        return inflater.inflate(R.layout.gameboard_fragment,container,false);
    }

    public static TextView gameboardHeader;
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState){
        LinearLayout root = (LinearLayout) view.findViewById(R.id.gameboard_linear_layout);

        view.setBackgroundResource(R.drawable.lobby_background);

        gameboardHeader = (TextView) view.findViewById(R.id.gameboard_header);

        GameView gv = new GameView(getContext());
        root.addView(gv);

    }


}
