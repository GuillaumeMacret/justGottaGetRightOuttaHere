using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class MatrixLine
{
    public int[] line;
    public MatrixLine(int [] line)
    {
        this.line = line;
    }
}

[System.Serializable]
public class LoadLevelJson
{
    public string Action;
    public List<int[]> Blocks;

}