using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameListCreator : MonoBehaviour
{
	public GameObject ContentPanel;
	public GameObject ListGamePrefab;

	private bool needToRefillList;
	private List<GameListElementModel> games;
	
    // Start is called before the first frame update
    void Start()
    {
		games = new List<GameListElementModel>();
		needToRefillList = false;
		UpdateGameList();
    }

	void Update() {
		if(needToRefillList) {
			for (int i = ContentPanel.transform.childCount - 1; i >= 0; --i) {
				Transform item = ContentPanel.transform.GetChild(i);
				item.SetParent(null);
			}
			if (games != null) {
				foreach (GameListElementModel gameElement in games) {
					GameObject newGame = Instantiate(ListGamePrefab);
					GameListItemController gameListItemController = newGame.GetComponent<GameListItemController>();
					gameListItemController.GameIDText.text = "" + gameElement.GameId;
					gameListItemController.NbPlayersText.text = "Players: " + gameElement.NbPlayers + "/4";
					newGame.transform.SetParent(ContentPanel.transform);
					newGame.transform.localScale = Vector3.one;
				}
			}
			needToRefillList = false;
		}
	}

	public void SetGameListAndUpdate(List<GameListElementModel> newGames) 
	{
		games = newGames;
		UpdateGameList();
	}

    void UpdateGameList()
    {
		needToRefillList = true;
    }
}
