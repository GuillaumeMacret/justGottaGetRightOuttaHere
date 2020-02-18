package com.example.justgottagetrightouttahere_client.Fragments;

import android.os.Bundle;
import android.text.Layout;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;

import androidx.fragment.app.Fragment;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.Views.GameView;

import static com.example.justgottagetrightouttahere_client.model.TilesSprites.tilesSpritesMap;

public class GameboardFragment extends Fragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle bundle){
        return inflater.inflate(R.layout.gameboard_fragment,container,false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState){
        LinearLayout root = (LinearLayout) view.findViewById(R.id.gameboard_linear_layout);

        root.addView(new GameView(getContext()));

    }


}
