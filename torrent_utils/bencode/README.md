
Python module / script to parse a torrent file to a python object.
I just wrote this for fun one evening, and to learn a bit more about the torrent file format.

See http://www.bittorrent.org/beps/bep_0003.html for more info.

The decode method accepts a bytes object and returns a list.

bencode.py is the module.
example.py is an example script that loads a torrent file and prettyprints the python object to a file.

Example:

```python
>>> import bencode
>>>
>>> with open("archlinux-2013.04.01-dual.iso.torrent", 'rb') as fh:
...     torrent_data = fh.read()
... 
>>> torrent = bencode.decode(torrent_data)
>>> 
>>> print("announce: ", torrent[0][b'announce'])
announce:  b'http://tracker.archlinux.org:6969/announce'
>>> print("name: ", torrent[0][b'info'][b'name'])
name:  b'archlinux-2013.04.01-dual.iso'
>>> 
```
