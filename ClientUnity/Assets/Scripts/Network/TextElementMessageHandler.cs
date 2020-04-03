using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TextElementMessageHandler : MonoBehaviour, IMessageHandler
{
    public FakeModel model;
    public void Handle(string s)
    {
        model.textValue = s;
    }
}
