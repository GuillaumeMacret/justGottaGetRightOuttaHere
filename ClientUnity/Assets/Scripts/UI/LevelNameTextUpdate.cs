using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LevelNameTextUpdate : MonoBehaviour
{
    private Text nameText;

    private void Awake()
    {
        nameText = GetComponent<Text>();
    }

    // Update is called once per frame
    void Update()
    {
        nameText.text = GameModel.LevelName;
    }
}
