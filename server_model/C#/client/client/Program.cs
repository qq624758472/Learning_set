using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net.Sockets;
using System.Net;
using System.Threading;



namespace client
{
    class Program
    {
        private static byte[] result = new byte[1024];
        static void Main(string[] args)
        {
            string serverIP = "127.0.0.1";
            //设定服务器IP地址 
            IPAddress ip = IPAddress.Parse(serverIP);
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                clientSocket.Connect(new IPEndPoint(ip, 4399)); //配置服务器IP与端口 
                Console.WriteLine("连接服务器成功");
            }
            catch
            {
                Console.WriteLine("连接服务器失败，请按回车键退出！");
                return;
            }
            //给服务器发送命令
            try
            {
                string sendMessage = "{\"cmd\":\"login\",\"username\":\"liu\",\"passwd\":\"111\"}";
                clientSocket.Send(Encoding.ASCII.GetBytes(sendMessage));
                Console.WriteLine("向服务器发送消息：{0}" + sendMessage);
                //Thread.Sleep(1000);//睡眠500毫秒，也就是0.5秒
                //clientSocket.Send(Encoding.ASCII.GetBytes(sendMessage));
            }
            catch
            {
                clientSocket.Shutdown(SocketShutdown.Both);
                clientSocket.Close();
                return;
            }

            //通过clientSocket接收数据
            int receiveLength = clientSocket.Receive(result);
            Console.WriteLine("接收服务器消息：{0}", Encoding.ASCII.GetString(result, 0, receiveLength));

            if(Encoding.ASCII.GetString(result, 0, receiveLength) == "loginSuccess")
            {
                Console.WriteLine("1");
            }
            else
            {
                Console.WriteLine("2");
            }

            clientSocket.Shutdown(SocketShutdown.Both);
            clientSocket.Close();
        }
    }
}
