function httpGet(u)
{
    var xmlhttp = null
    if (window.XMLHttpRequest)
        xmlhttp = new XMLHttpRequest()
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState == 4 && xmlhttp.status==200){
            console.log(xmlhttp.responseText)
            return xmlhttp.responseText
        }
    }
    xmlhttp.open("GET", u, false );
    xmlhttp.send();    
}

function gatherer_main() {
    httpGet("https://www.socks-proxy.net/")
}

document.addEventListener('DOMContentLoaded', gatherer_main, false);