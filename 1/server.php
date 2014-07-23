<!--  

This Script is to store GPS strings over header as Latitude & Longitude
store them into a TXT file under the name of the day. If no Lat&Long information
on the header, reads the TXT file according to the date, and after that post them
into a Google Maps V3 Api map.
Author: N4rf - 

-->

<?php

    if (!empty($_GET['latitude']) && !empty($_GET['longitude'])) {

        function getParameter($par, $default = null){
            if (isset($_GET[$par]) && strlen($_GET[$par])) return $_GET[$par];
            elseif (isset($_POST[$par]) && strlen($_POST[$par])) 
                return $_POST[$par];
            else return $default;
        }
        
        	$day=date("d");
		  	$month=date("m");
			$year=date("y");
			$fecha=$day.$month."20".$year;
			$filename=$fecha.'.txt';

        $file = $filename;
        $lat = getParameter("latitude");
        $lon = getParameter("longitude");

        $person = $lat.",".$lon."n";
        
        echo "
            DATA:n
            Latitude: ".$lat."n
            Longitude: ".$lon;

        if (!file_put_contents($file, $person, FILE_APPEND | LOCK_EX))
            echo "nt Error saving Datan";
        else echo "nt Data Saven";
    }
    else {

?>

<!DOCTYPE html>
<html>
    
<head>

    <!-- Load Jquery -->

    <script language="JavaScript" type="text/javascript" src="jquery-1.10.1.min.js"></script>

    <!-- Load Google Maps Api -->

    <!-- IMPORTANT: change the API v3 key -->

    <script src="http://maps.googleapis.com/maps/api/js?key=YOUR_API_KEY&sensor=false"></script>

    <!-- Initialize Map and markers -->

    <script type="text/javascript">
        var myCenter=new google.maps.LatLng(-31.40306,-64.19827);
        var marker;
        var map;
        var mapProp;

        function initialize()
        {
            mapProp = {
              center:myCenter,
              zoom:15,
              mapTypeId:google.maps.MapTypeId.ROADMAP
              };
            setInterval('mark()',30000);
        }

        function mark()
        {
        	
        		//adquirimos las fechas para generar el archivo del dia
        		var f = new Date();
        		
        		
        		if(f.getDate() < 10 && (f.getMonth() 1)<10){
        		var fecha =("0" f.getDate() "0" (f.getMonth()  1)  f.getFullYear());
				}else if(f.getDate() > 10 && (f.getMonth() 1)<10){
        		var fecha =(f.getDate() "0" (f.getMonth()  1)  f.getFullYear());
				}else if(f.getDate() < 10 && (f.getMonth() 1)>10){
        		var fecha =("0" f.getDate() (f.getMonth()  1)  f.getFullYear());
				}else if(f.getDate() > 10 && (f.getMonth() 1)>10){
        		var fecha =(f.getDate() (f.getMonth()  1)  f.getFullYear());
				}
				
            map=new google.maps.Map(document.getElementById("googleMap"),mapProp);
            var file = fecha ".txt";
            $.get(file, function(txt) { 
                var lines = txt.split("n");
                for (var i=0;i<lines.length;i  ){
                    console.log(lines[i]);
                    var words=lines[i].split(",");
                    if ((words[0]!="")&&(words[1]!=""))
                    {
                        marker=new google.maps.Marker({
                              position:new google.maps.LatLng(words[0],words[1]),
                              });
                        marker.setMap(map);
                        map.setCenter(new google.maps.LatLng(words[0],words[1]));
                    }
                }
                marker.setAnimation(google.maps.Animation.BOUNCE);
            });

        }

        google.maps.event.addDomListener(window, 'load', initialize);
    </script>
</head>

<body>
		<body background="background.png">

    <?php
        echo '    

        <!-- Draw information table and Google Maps div -->

        <div>
            <center><br />
                <b> Posicion Actual</b><br /><br />
                <br /><br />
                <div id="googleMap" style="width:800px;height:700px;"></div>
            </center>
        </div>';
    ?>
</body>
</html>

<?php } ?>

