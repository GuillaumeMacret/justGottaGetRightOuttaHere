using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FakeModel : MonoBehaviour
{
    public string textValue;
    public Text textfield;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        textfield.text = textValue;
    }
}
