package com.example.justgottagetrightouttahere_client.Fragments;

import android.content.Intent;
import android.media.Image;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.fragment.app.Fragment;

import com.example.justgottagetrightouttahere_client.Activities.GameActivity;
import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.Views.GameView;
import com.example.justgottagetrightouttahere_client.model.Player;

public class LobbyPlayerFragment extends Fragment {
    public Player player;
    public boolean isMyPlayer = true;
    protected int currentCharacter = 0;
    private ImageView playerImage;
    private TextView playerIDTextView;
    private TextView abilityTextView;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle bundle){
        return inflater.inflate(R.layout.lobby_player_fragment,container,false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState){
        playerImage = (ImageView) view.findViewById(R.id.player_image_view);
        playerIDTextView = (TextView) view.findViewById(R.id.player_id_text_view);
        abilityTextView = (TextView) view.findViewById(R.id.player_ability_text_view);

        Button switchCharacter = (Button) view.findViewById(R.id.switch_character_button);

        switchCharacter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                switchToNextCharacter();
            }
        });
    }

    public void switchToNextCharacter() {
        if(isMyPlayer) {
            // Set next character number
            currentCharacter = (currentCharacter + 1) % 4;
            switch (currentCharacter) {
                case 0:
                    playerImage.setImageResource(R.drawable.player0_idle);
                    abilityTextView.setText(R.string.ability_player0);
                    break;
                case 1:
                    playerImage.setImageResource(R.drawable.player1_idle);
                    abilityTextView.setText(R.string.ability_player1);
                    break;
                case 2:
                    playerImage.setImageResource(R.drawable.player2_idle);
                    abilityTextView.setText(R.string.ability_player2);
                    break;
                case 3:
                    playerImage.setImageResource(R.drawable.player3_idle);
                    abilityTextView.setText(R.string.ability_player3);
                    break;
            }
        }
    }
}
