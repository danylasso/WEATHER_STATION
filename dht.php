<?php
class dht11{
 public $link='';
 function __construct($temperature, $humidity, $pressure, $altitude, $lluvia){
  $this->connect();
  $this->storeInDB($temperature, $humidity, $pressure, $altitude, $lluvia);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'temphumidnew') or die('Cannot select the DB');
 }
 
 function storeInDB($temperature, $humidity, $pressure, $altitude, $lluvia){
  $query = "insert into dht11 set humidity='".$humidity."', temperature='".$temperature."', pressure='".$pressure."',altitude='".$altitude."', lluvia='".$lluvia."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['temperature'] != '' and  $_GET['humidity'] != '' and  $_GET['pressure'] != '' and  $_GET['altitude'] != '' and  $_GET['lluvia'] != '' ){
 $dht11=new dht11($_GET['temperature'],$_GET['humidity'],$_GET['pressure'],$_GET['altitude'],$_GET['lluvia'] );
}
?>