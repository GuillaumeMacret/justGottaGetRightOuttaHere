package com.example.justgottagetrightouttahere_client.Adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.GameInfo;

import org.w3c.dom.Text;

import java.util.ArrayList;

public class GameInfoAdapter extends ArrayAdapter<GameInfo> {

    private ArrayList<GameInfo> gameList;
    Context mContext;

    private static class ViewHolder {
        TextView gameIdTextView;
        TextView nbPlayersTextView;
    }

    public GameInfoAdapter(ArrayList<GameInfo> data, Context context) {
        super(context, R.layout.game_info_row, data);
        this.gameList = data;
        mContext = context;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        GameInfo gameInfo = getItem(position);
        ViewHolder viewHolder;

        View result;

        if(convertView == null) {
            viewHolder = new ViewHolder();
            LayoutInflater inflater = LayoutInflater.from(getContext());
            convertView = inflater.inflate(R.layout.game_info_row, parent, false);
            viewHolder.gameIdTextView = (TextView) convertView.findViewById(R.id.gamelist_id_text_view);
            viewHolder.nbPlayersTextView = (TextView) convertView.findViewById(R.id.gamelist_nb_players_text_view);
            result = convertView;
            convertView.setTag(viewHolder);
        }
        else {
            viewHolder = (ViewHolder) convertView.getTag();
            result = convertView;
        }

        viewHolder.gameIdTextView.setText("Game ID: " + gameInfo.id);
        viewHolder.nbPlayersTextView.setText("Players: " + gameInfo.nbPlayers + "/4");

        return convertView;
    }
}
