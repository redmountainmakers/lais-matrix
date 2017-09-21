const char index_html[] PROGMEM = R"(<head>
<title>WiFi LED Demo</title>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
.color-sel { width: 24px; height: 24px; text-align: center;}
.pixel-sel { width: 48px; height: 48px; background-color: black; }
button { font-size: 16px; }
p { font-size:18px; font-style:normal; line-height:0px; }
table { border-color:lightgray; border-collapse:collapse; }
</style>
</head>
<body>
<script>
var nh=12,i,j,cc='rgb(255, 255, 255)';
var cc_elem;
function w(s) { document.write(s); }
function id(s){ return document.getElementById(s); }
function mark(e) {e.innerHTML='&#10004;';}
function cs_click(e) {
var bg=e.target.style.background;
cc_elem.innerHTML='';
mark(e.target);
cc=bg;
cc_elem=e.target;
}
w('<table border=1>');
idx=0;
for(i=0;i<2;i++) {
w('<tr>');
for(j=0;j<nh;j++,idx++) {
w('<td class=color-sel id=cs'+idx+'></td>');
var obj=id('cs'+idx);
if(i==0) { var hue=(j/nh)*360; obj.style.background='hsl('+hue+',100%,50%)'; }
else { var lit=(j/(nh-1))*100; obj.style.background='hsl(0,0%,'+lit+'%)'; }
obj.addEventListener('click',cs_click);
}
w('</tr>');
}
w('</table>');
cc_elem=id('cs'+(2*nh-1));
mark(cc_elem);
</script>
<hr />
<p>Set pixel color: <button id='btn_fill'>Fill</button> <button id='btn_clear'>Clear</button> <button id='btn_submit'><b>Submit</b></button></p>
<script>
function ps_click(e) { if(e.target.style.background==cc) e.target.style.background='rgb(0,0,0)'; else e.target.style.background=cc; }
w('<table border=1 style="border-collapse: separate; border-spacing: 2px;">');
idx=0;
for(i=0;i<7;i++) {
w('<tr>');
for(j=0;j<5;j++,idx++) {
w('<td class=pixel-sel id=ps'+idx+'></td>');
id('ps'+idx).addEventListener('click',ps_click);
}
w('</tr>');
}
w('</table>');
</script>
<p>Pre-defined patterns:</p>
<button id='btn_smiley'>Smiley</button> <button id='btn_pumpkin'>Pumpkin</button> <button id='btn_skull'>Skull</button> <button id='btn_heart'>Heart</button> <button id='btn_checker'>Checker</button>
<p><input type=checkbox id='cb_comm'> Show HTTP Get command
</p>
<label id='lbl_comm'></label>
<script>
function rgb2cc(rgb) {
if(!rgb) return '000000';
rgb = rgb.match(/^rgba?\((\d+),\s*(\d+),\s*(\d+)(?:,\s*(\d+))?\)$/);
function hex(x) { return ('0' + parseInt(x).toString(16)).slice(-2); }
return '' + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3]);
}
id('btn_fill').addEventListener('click',function(e) {
for(i=0;i<35;i++) id('ps'+i).style.background = cc;
});
id('btn_clear').addEventListener('click',function(e) {
for(i=0;i<35;i++) id('ps'+i).style.background = 'rgb(0, 0, 0)';
});
function send_comm(comm) {
var xhr=new XMLHttpRequest();
xhr.onreadystatechange=function() {
if(xhr.readyState==4 && xhr.status==200) {
var jd=JSON.parse(xhr.responseText);
// jd stores the response data
}
};
comm = 'http://192.168.4.1'+comm;
xhr.open('GET',comm,true);
xhr.send();
}
function show_comm(comm) {
comm = 'http://192.168.4.1'+comm;
if(id('cb_comm').checked) id('lbl_comm').innerHTML=comm;
else id('lbl_comm').innerHTML='';
}
id('btn_submit').addEventListener('click', function(e) {
var comm='/cc?pixels=';
for(i=0;i<35;i++) comm+=rgb2cc(id('ps'+i).style.background);
show_comm(comm);
send_comm(comm);
});
function set_icon(icon, fg, bg) {
for(i=0;i<35;i++) id('ps'+i).style.background=(icon[i]>0)?fg:bg;
}
function set_icon2(icon) {
for(i=0;i<35;i++) id('ps'+i).style.background=icon[i];
}
id('btn_smiley').addEventListener('click', function(e) {
var smiley=[0,1,1,1,0,
1,0,0,0,1,
1,1,0,1,1,
1,0,0,0,1,
1,1,1,1,1,
1,0,0,0,1,
0,1,1,1,0];
set_icon(smiley, '#00FF00', '#000000');
});
id('btn_heart').addEventListener('click', function(e) {
var heart=[0,0,0,0,0,
0,1,0,1,0,
1,1,1,1,1,
1,1,1,1,1,
0,1,1,1,0,
0,0,1,0,0,
0,0,0,0,0];
set_icon(heart, '#FF0000', '#000020');
});
id('btn_checker').addEventListener('click', function(e) {
var heart=[1,0,1,0,1,
0,1,0,1,0,
1,0,1,0,1,
0,1,0,1,0,
1,0,1,0,1,
0,1,0,1,0,
1,0,1,0,1];
set_icon(heart, '#FF0000', '#000020');
});
id('btn_skull').addEventListener('click', function(e) {
var skull=[0,1,1,1,0,
1,0,0,0,1,
1,1,0,1,1,
1,0,0,0,1,
0,1,0,1,0,
0,1,0,1,0,
0,1,1,1,0];
set_icon(skull, '#0000FF', '#000000');
});
id('btn_pumpkin').addEventListener('click', function(e) {
var pumpkin=['#000000','#008000','#000000','#000000','#000000',
'#000000','#000000','#008000','#000000','#000000',
'#000000','#802000','#802000','#802000','#000000',
'#802000','#000000','#000000','#000000','#802000',
'#802000','#000000','#000000','#000000','#802000',
'#802000','#000000','#000000','#000000','#802000',
'#000000','#802000','#802000','#802000','#000000'];
set_icon2(pumpkin);
});
</script>
</body>)";

