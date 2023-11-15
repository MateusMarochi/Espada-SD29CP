using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class TCPServer : MonoBehaviour
{
    public Cube cube;

    private TcpListener tcpListener;
    private Thread tcpListenerThread;
    private TcpClient connectedTcpClient;

    void Start()
    {
        tcpListenerThread = new Thread(new ThreadStart(ListenForIncomingRequests));
        tcpListenerThread.IsBackground = true;
        tcpListenerThread.Start();
    }

    private void ListenForIncomingRequests()
    {
        try
        {
            tcpListener = new TcpListener(IPAddress.Any, 8052);
            tcpListener.Start();
            Byte[] bytes = new Byte[1024];
            while (true)
            {
                using (connectedTcpClient = tcpListener.AcceptTcpClient())
                {
                    using (NetworkStream stream = connectedTcpClient.GetStream())
                    {
                        int length;
                        while ((length = stream.Read(bytes, 0, bytes.Length)) != 0)
                        {
                            var incomingData = new byte[length];
                            Array.Copy(bytes, 0, incomingData, 0, length);
                            string clientMessage = Encoding.ASCII.GetString(incomingData);
                            ProcessData(clientMessage);
                        }
                    }
                }
            }
        }
        catch (SocketException socketException)
        {
            Debug.LogError("SocketException " + socketException.ToString());
        }
    }

    private void ProcessData(string data)
    {
        string[] splitData = data.Split(',');
        if (splitData.Length == 4)
        {
            float yaw = float.Parse(splitData[1]) / 100;
            float pitch = float.Parse(splitData[2]) / 100;
            float roll = float.Parse(splitData[3]) / 100;

            Quaternion rotation = Quaternion.Euler(pitch, yaw, roll);
            print(pitch + ", " + yaw + ", " + roll);
            cube.SetRotation(rotation);
        }
    }

    void OnApplicationQuit()
    {
        if (tcpListener != null)
        {
            tcpListener.Stop();
        }
    }
}
