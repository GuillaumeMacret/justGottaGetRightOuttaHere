using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TimerTextUpdate : MonoBehaviour
{
    private Text timeText;

    private void Awake()
    {
        timeText = GetComponent<Text>();
    }

    // Update is called once per frame
    void Update()
    {
        float timeF = GameModel.GameTimer;
        TimeSpan time = TimeSpan.FromSeconds(timeF);
        timeText.text = time.ToString(@"mm\:ss");
    }
}
