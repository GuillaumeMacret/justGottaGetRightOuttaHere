using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameListCreator : MonoBehaviour
{
	public GameObject ContentPanel;
	public GameObject ListGamePrefab;

	private List<GameListElementModel> games;
	
    // Start is called before the first frame update
    void Start()
    {
		games = new List<GameListElementModel>();
		//TODO : Remove this
		games.Add(new GameListElementModel(1547, 3));
		games.Add(new GameListElementModel(02, 2));
		games.Add(new GameListElementModel(447, 1));
		games.Add(new GameListElementModel(1547, 3));
		//
		UpdateGameList();
    }

	public void SetGameListAndUpdate(List<GameListElementModel> newGames) 
	{
		games = newGames;
		UpdateGameList();
	}

    void UpdateGameList()
    {
		for(int i = ContentPanel.transform.childCount - 1; i >= 0 ; --i) 
		{
			Transform item = ContentPanel.transform.GetChild(i);
			item.parent = null;
			Destroy(item);
		}
        foreach(GameListElementModel gameElement in games) 
		{
			GameObject newGame = Instantiate(ListGamePrefab);
			GameListItemController gameListItemController = newGame.GetComponent<GameListItemController>();
			gameListItemController.GameIDText.text = "" + gameElement.GameId;
			gameListItemController.NbPlayersText.text = "Players: " + gameElement.NbPlayers + "/4";
			newGame.transform.SetParent(ContentPanel.transform);
			newGame.transform.localScale = Vector3.one;
		}
    }
}
