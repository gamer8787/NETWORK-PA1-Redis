redis-cli에서
make_resp_form 함수는 fgets를 이용하여 stdin을 한줄씩 읽고 폼에 맞게 서버에 보내는 것이고
printf_read_message 함수는 write를 하고 read한 내용을 swtich문을 통해 
$,: 등등이 오는 경우에 따라 print해주는 함수입니다.

redis-server는 while(1)을 이용해 client accept write,read close를 하고
command가 get,set 중에 무엇인지 strncmp를 이용해 분류하고 각각에 맞는 함수를 구현하였습니다.
몇 몇 함수는 map<string, string> m을 이용하여 해결하였습니다.