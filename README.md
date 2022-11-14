# WSChatProClient
A client used to sign in a FTP server while can chat with other users

## 存在的问题
1. 如果增添差错处理模块，需要定义一个发送消息或者利用`FD_WRITE`，将目前所有的发送报文的部分，从各个控件或者模块的事件处理函数中统一利用发送消息的处理函数执行，各个控件的消息处理函数仅负责将数据提交，发出一个消息，然后由发送模块发送，并进行差错处理的控制。
2. group id按照字符串进行处理，即4个字符。
