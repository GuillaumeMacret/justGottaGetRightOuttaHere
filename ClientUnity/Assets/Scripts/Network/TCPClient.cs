using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class TCPClient
{
    private static TCPClient INSTANCE;
    public static TCPClient GetInstance()
    {
        if (INSTANCE == null) INSTANCE = new TCPClient();
        return INSTANCE;
    }

    public static bool socketConnected = false;
    public static void ConnectIfNotConnected()
    {
        if (socketConnected) return;
        ConnectToTcpServer();
    }

    #region private members 	
    private static TcpClient socketConnection;
    private static Thread clientReceiveThread;
    private const int BUFFER_SIZE = 50000;

    public static string serverAddr = "";
    public const string DEFAULT_SERVER_ADDR = "norcisrasp.ddns.net";
    private const int SERVER_PORT = 1789;
    static IMessageHandler  m_MessageHandler = null;
    #endregion


    /// <summary> 	
    /// Sets the class that will handle incomming messages
    /// </summary> 	
    public static void SetMessageHandler(IMessageHandler handler)
    {
        m_MessageHandler = handler;
    }

    /// <summary> 	
    /// Setup socket connection. 	
    /// </summary> 	
    private static void ConnectToTcpServer()
    {
        if (serverAddr.Length <= 1) serverAddr = DEFAULT_SERVER_ADDR;
        Debug.Log("Trying to connect to " + serverAddr + ":" + SERVER_PORT);
        try
        {
            clientReceiveThread = new Thread(new ThreadStart(ListenForData));
            clientReceiveThread.IsBackground = true;
            clientReceiveThread.Start();
            socketConnected = true;
            Debug.Log("Connected !");
        }
        catch (Exception e)
        {
            Debug.Log("On client connect exception " + e);
        }
    }
    /// <summary> 	
    /// Runs in background clientReceiveThread; Listens for incomming data. 	
    /// </summary>     
    private static void ListenForData()
    {
        try
        {
            socketConnection = new TcpClient(serverAddr, SERVER_PORT);
            Byte[] bytes = new Byte[BUFFER_SIZE];
            StringBuilder stringBuilder = new StringBuilder();
            while (true)
            {
                // Get a stream object for reading 				
                using (NetworkStream stream = socketConnection.GetStream())
                {
                    bytes = new Byte[BUFFER_SIZE];
                    int length;
                    // Read incomming stream into byte arrary. 					
                    while ((length = stream.Read(bytes, 0, bytes.Length)) != 0)
                    {
                        var incommingData = new byte[length];
                        Array.Copy(bytes, 0, incommingData, 0, length);
                        // Convert byte array to string message. 						
                        string serverMessage = Encoding.ASCII.GetString(incommingData);
                        Debug.Log("Server message received as: " + serverMessage);
                        foreach(char c in serverMessage)
                        {
                            if(c == ';')
                            {
                                if(m_MessageHandler != null)
                                {
                                    m_MessageHandler.Handle(stringBuilder.ToString());
                                }
                                else
                                {
                                    Debug.LogWarning("No handler set to handle the message <" + stringBuilder.ToString() + ">");
                                }
                                stringBuilder.Clear();
                            }
                            else if(c != '\n')
                            {
                                stringBuilder.Append(c);
                            }
                        }
                        
                        /*
                        if(m_MessageHandler != null && stringBuilder[stringBuilder.Length - 2] == ';')
                        {
                            string JSONonly = stringBuilder.Remove(stringBuilder.Length - 2, 2).ToString();
                            Debug.Log("Handling message : <" + serverMessage.Remove(serverMessage.Length - 2,2) + ">");
                            m_MessageHandler.Handle(JSONonly);
                            stringBuilder = new StringBuilder();
                        }
                        */
                    }
                }
            }
        }
        catch (SocketException socketException)
        {
            Debug.Log("Socket exception: " + socketException);
        }
    }
    /// <summary> 	
    /// Send message to server using socket connection. 	
    /// </summary> 	
    public static bool SendMessage(string clientMessage)
    {
        if (socketConnection == null)
        {
			Debug.LogWarning("Could not send message: Socket is null");
            return false;
        }
        try
        {
            // Get a stream object for writing. 			
            NetworkStream stream = socketConnection.GetStream();
            if (stream.CanWrite)
            {
                // Convert string message to byte array.                 
                byte[] clientMessageAsByteArray = Encoding.ASCII.GetBytes(clientMessage);
                // Write byte array to socketConnection stream.                 
                stream.Write(clientMessageAsByteArray, 0, clientMessageAsByteArray.Length);
                stream.Flush();
                Debug.Log("Client sent " + clientMessage);
				return true;
            }
        }
        catch (SocketException socketException)
        {
            Debug.Log("Socket exception: " + socketException);
        }
		return false;
    }

	/// <summary> 	
	/// Returns true if socket is connected
	/// </summary> 	
	public static bool IsSocketConnected() 
	{
		return socketConnection != null;
	}
	
}