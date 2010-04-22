#!/usr/bin/env python

"""

For those of you who might be getting the error:
QPainter::begin: Paint device returned engine == 0, type: 3
There are a couple possible reasons:
- The page is greater than 32,768 pixels (2^15 px) in any dimension (http://doc.trolltech.com/4.5/qpainter.html#limitations)
- The page is framed and messing with the image dimensions.

"""

import sys
import signal

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import QWebPage

def onLoadFinished(result):
    if not result:
        print "Request failed"
        sys.exit(1)

    # Set the size of the (virtual) browser window
    #webpage.setViewportSize(webpage.mainFrame().contentsSize())
    webpage.setViewportSize(QSize(800,600))

    # Paint this frame into an image
    """
    image = QImage(webpage.viewportSize(), QImage.Format_ARGB32)
    painter = QPainter(image)
    """
    wframe = webpage.mainFrame()
    """
    wframe.render(painter)


    painter.end()
    image.save("output_ddd2.png")
    """

    si = wframe.evaluateJavaScript('ol_serviceinfo(map);').toPyObject()
    print 'service title : %s' % si.get(QString('title')) 
    for qi, layer in si.get(QString('layers'),[]).iteritems():
      print "  [%s] %s" % (qi, layer.get(QString('name')))
    sys.exit(0)


app = QApplication(sys.argv)
signal.signal(signal.SIGINT, signal.SIG_DFL)

#url = QUrl("http://localhost/docs/")
url = QUrl("http://localhost/web/osm.html")
#url= QUrl.fromLocalFile("/home/nico/projekte/openlayers-wms/index.html")

webpage = QWebPage()
webpage.connect(webpage, SIGNAL("loadFinished(bool)"), onLoadFinished)
webpage.mainFrame().load(url)

sys.exit(app.exec_())
