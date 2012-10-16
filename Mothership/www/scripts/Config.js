function updateConfiguration()
{
  if(message.id == '')
  {
    alert('Attemping to submit to null target string, doing nothing');
    return;
  }
  console.log('Sending configuration update requests to targets: ' + message.id);
  now.UpdateConfiguration();
}

function setTarget(source, target, group)
{
  if(group == 'true' || group == true)
  {
    if(document.getElementById(source).checked)
    {
      message.id = target + ':';
      var targets = target.split(':');

      for(var i in document.getElementById('groupsList').childNodes)
      {
        if(document.getElementById('groupcheck' + i) != undefined && document.getElementById('groupcheck' + i).id.indexOf(source) == -1)
        {
          document.getElementById('groupcheck' + i).setAttribute('disabled', true);
        }
      }
      for(var i in document.getElementById('clientsList').childNodes)
      {
        if(document.getElementById('check' + i) != undefined)
        {
          document.getElementById('check' + i).checked = false;
          document.getElementById('check' + i).setAttribute('disabled', true);
        } 
      }
    }
    else
    {
      message.id = '';
      for(var i in document.getElementById('groupsList').childNodes)
      {
        if(document.getElementById('groupcheck' + i) != undefined && document.getElementById('groupcheck' + i).id.indexOf(source) == -1)
        {
          document.getElementById('groupcheck' + i).removeAttribute('disabled');
        }
      }
      for(var i in document.getElementById('clientsList').childNodes)
      {
        if(document.getElementById('check' + i) != undefined)
        {
          document.getElementById('check' + i).removeAttribute('disabled');
        } 
      }
    }
  }
  else
  {
    if(document.getElementById(source).checked)
    {
      message.id += target + ':';
    }
    else
    {
      var regex = new RegExp(target + ':', 'i');
      message.id = message.id.replace(regex, '');
    }
  }
  document.getElementById('showtargets').value = message.id.replace(new RegExp(':', 'g'), ',').substr(0, message.id.length - 1);
}
 
now.UpdateConfiguration = function()
{
  //construct json here
  //message type: updateConfiguration
  message.type = 'updateConfiguration';
  
  message.TCP_TIMEOUT = document.getElementsByName('TCP_TIMEOUT')[0].value;
  message.TCP_CHECK_FREQ = document.getElementsByName('TCP_CHECK_FREQ')[0].value;
  message.CLASSIFICATION_TIMEOUT = document.getElementsByName('CLASSIFICATION_TIMEOUT')[0].value;
  message.K = document.getElementsByName('K')[0].value;
  message.EPS = document.getElementsByName('EPS')[0].value;
  message.CLASSIFICATION_THRESHOLD = document.getElementsByName('CLASSIFICATION_THRESHOLD')[0].value;
  message.MIN_PACKET_THRESHOLD = document.getElementsByName('MIN_PACKET_THRESHOLD')[0].value;
  
  if(document.getElementById('clearHostileYes').checked)
  {
    message.CLEAR_AFTER_HOSTILE_EVENT = '1';  
  }
  else
  {
    message.CLEAR_AFTER_HOSTILE_EVENT = '0';
  }
  
  message.CUSTOM_PCAP_FILTER = document.getElementsByName('CUSTOM_PCAP_FILTER')[0].value;
  
  if(document.getElementById('customPcapYes').checked)
  {
    message.CUSTOM_PCAP_MODE = '1';
  }
  else
  {
    message.CUSTOM_PCAP_MODE = '0';
  }
  
  message.CAPTURE_BUFFER_SIZE = document.getElementsByName('CAPTURE_BUFFER_SIZE')[0].value;
  message.SAVE_FREQUENCY = document.getElementsByName('SAVE_FREQUENCY')[0].value;
  message.DATA_TTL = document.getElementsByName('DATA_TTL')[0].value;
  
  if(document.getElementById('dmEnabledYes').checked)
  {
    message.DM_ENABLED = '1';
  }
  else
  {
    message.DM_ENABLED = '0';
  }
  
  message.SERVICE_PREFERENCES = document.getElementsByName('SERVICE_PREFERENCES')[0].value;
  
  if(/^0:[0-7](\\+|\\-)?;1:[0-7](\\+|\\-)?;2:[0-7](\\+|\\-)?;$/.test(message.SERVICE_PREFERENCES) == false)
  {
    document.getElementById('SERVICE_PREFERENCES').value = replace;
    alert('Service Preferences string is not formatted correctly.');
    return;
  }
  
  now.MessageSend(message);
}