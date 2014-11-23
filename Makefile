# The authors of this work have released all rights to it and placed it
# in the public domain under the Creative Commons CC0 1.0 waiver
# (http://creativecommons.org/publicdomain/zero/1.0/).
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# Retrieved from: http://en.literateprograms.org/Hello_World_(C,_Cairo)?oldid=10388

CFLAGS=-Wall -g -I/usr/X11R6/include `pkg-config --cflags cairo pangocairo`
LDFLAGS=-Wall -g `pkg-config --libs cairo pangocairo` -L/usr/X11R6/lib -lX11

all: lubare

lubare: main.o
	cc -o lubare ${LDFLAGS} main.o

main.o: main.c
	cc ${CFLAGS} -c main.c

clean:
	rm lubare *.o
