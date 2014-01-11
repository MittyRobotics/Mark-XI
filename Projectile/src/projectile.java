import java.awt.*;
import javax.swing.JApplet;

@SuppressWarnings("serial")
public class projectile extends JApplet implements Runnable
{
	int timeStep = 3; // milliseconds

	double x, y, vx, vy, v, theta, range, dt, dragCoef, fluidDens, area, mass, radius, acc, alpha;
	double x2, y2, vy2, vx2, range1, range2, largestTheta, largestRange1, largestRange2, prevRange1, prevRange2;
	boolean slowmode = false, complexMode = true, largeRangeFound = false, foundLargest = false;

	Thread t = new Thread(this);

	Graphics bufferGraphics;
	Image offscreen;
	Dimension dim;

	public void init()
	{
		v = 100.0;
		theta = 30.0;
		x = 100.0;
		x2 = 100.0;
		y = 500.0;
		y2 = 500.0;

		radius = 0.1;
		alpha = theta;
		largestTheta = theta;
		mass = 1.0;
		dragCoef = 0.3;
		fluidDens = 1.275;
		area = Math.PI * radius * radius;

		acc = (1.0 / 2 * fluidDens * dragCoef * area * v * v) / mass;

		// compute vx, vy as functions of v, theta
		vx = v * Math.cos(Math.toRadians(theta));
		vy = -v * Math.sin(Math.toRadians(theta));
		vx2 = v * Math.cos(Math.toRadians(theta));
		vy2 = -v * Math.sin(Math.toRadians(theta));

		dt = timeStep / 1000.0;

		t.start();
		resize(1280, 900);

		dim = getSize();
		offscreen = createImage(dim.width, dim.height);
		bufferGraphics = offscreen.getGraphics();
		System.out.println(area);
	}
	public void vadimStop() 
	{
		while (true)
		{}
	}

	public void run()
	{
		try
		{
			while (true)
			{
				if (complexMode)
				{
					while (y <= 500 || y2 <= 500)
					{
						v = Math.sqrt((vx * vx) + (vy * vy));

						acc = (1.0 / 2 * fluidDens * dragCoef * area * v * v) / mass;
						alpha = Math.atan2(vy, vx);
						// update vx, vy, x, y

						// if the projectile is “above ground”, update range

						vy = vy + (9.8 * dt) - (acc * Math.sin(alpha) * dt);
						vx = vx - acc * Math.cos(alpha) * dt;

						vy2 = vy2 + (9.8 * dt);

						x = x + vx * dt;
						y = y + vy * dt;

						x2 = x2 + vx2 * dt;
						y2 = y2 + vy2 * dt;

						// if (vx <= 0)
						// vx = 0;
						if (y <= 500)
							range1 = x - 100;
						if (y2 <= 500)
							range2 = x2 - 100;

						repaint();
						Thread.sleep(timeStep);
					}

					if (range1 > prevRange1)
					{
						largestRange1 = range1;
						largestRange2 = range2;
						largestTheta = theta;
						theta++;
						if (largeRangeFound)
						{
							System.out.println("Largest range with drag happens at: " + theta + " degrees.");
							foundLargest = true;
							repaint();
							Thread.sleep(100000);
							vadimStop();
						}
					}
					if (range1 < prevRange1)
					{
						largestRange1 = prevRange1;
						largestRange2 = prevRange2;
						largeRangeFound = true;
					}
					if (range1 < prevRange1 || slowmode == true)
					{
						slowmode = true;
						theta -= 0.1;
					}
					
					prevRange1 = range1;
					prevRange2 = range2;
					v = 100.0;
					x = 100.0;
					x2 = 100.0;
					y = 500.0;
					y2 = 500.0;
					vx = v * Math.cos(Math.toRadians(theta));
					vy = -v * Math.sin(Math.toRadians(theta));
					vx2 = v * Math.cos(Math.toRadians(theta));
					vy2 = -v * Math.sin(Math.toRadians(theta));
				} else
				{
					while (y <= 500 || y2 <= 500)
					{
						v = Math.sqrt((vx * vx) + (vy * vy));

						acc = (1.0 / 2 * fluidDens * dragCoef * area * v * v) / mass;
						alpha = Math.atan2(vy, vx);
						// update vx, vy, x, y

						// if the projectile is “above ground”, update range

						vy = vy + (9.8 * dt) - (acc * Math.sin(alpha) * dt);
						vx = vx - acc * Math.cos(alpha) * dt;

						vy2 = vy2 + (9.8 * dt);

						x = x + vx * dt;
						y = y + vy * dt;

						x2 = x2 + vx2 * dt;
						y2 = y2 + vy2 * dt;

						// if (vx <= 0)
						// vx = 0;
						if (y <= 500)
							range1 = x - 100;
						
						if (y2 <= 500)
							range2 = x2 - 100;

						repaint();
						Thread.sleep(timeStep);
					}
				}
			}
		} catch (InterruptedException ex)
		{
		}
	}

	public void paint(Graphics g)
	{
		bufferGraphics.setColor(Color.blue);
		bufferGraphics.fillOval((int) x, (int) y, 15, 15);
		bufferGraphics.setColor(Color.red);
		bufferGraphics.fillOval((int) x2, (int) y2, 15, 15);
		bufferGraphics.setColor(Color.white);
		bufferGraphics.fillRect(499, 0, 500, 130);
		bufferGraphics.setColor(Color.black);
		bufferGraphics.drawString("Range1: " + range1, 500, 50);
		bufferGraphics.drawString("Current Theta: " + theta, 500, 75);
		bufferGraphics.drawString("Range2: " + range2, 500, 100);
		bufferGraphics.drawString("LargestTheta: " + largestTheta, 500, 125);
		bufferGraphics.setColor(Color.green);
		bufferGraphics.fillRect(0, 515, 2000, 50);
		if (foundLargest)
		{
			bufferGraphics.setColor(Color.MAGENTA);
			bufferGraphics.drawString("Largest range with drag happens at: " + theta + " degrees.", 500, 300);
		}

		g.drawImage(offscreen, 0, 0, this);

		showStatus("Range1: " + range1 + "     Range2: " + range2 + "     Drag: " + acc);
	}

	public void update(Graphics g)
	{
		paint(g);
	}
}