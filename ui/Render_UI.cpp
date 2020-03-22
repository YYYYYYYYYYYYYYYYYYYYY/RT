#include "Render_UI.h"
#include "ui_Render_UI.h"
#include "qfiledialog.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "x3dParser.h"
#include "Renderer.h"
#include "BasicShape.h"

QString *curr_dirr;
Parser * p = nullptr;
Render_UI::Render_UI(QWidget *parent) : QMainWindow(parent)
{
	curr_dirr = new QString();
	ui.setupUi(this);
	connect(ui.open_button, &QPushButton::clicked, this, &Render_UI::browse);
	connect(ui.render_button, &QPushButton::clicked, this, &Render_UI::render);
}

void Render_UI::browse()
{
	*curr_dirr = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("*.x3d *.xml *.html"));
	p = new Parser(curr_dirr->toStdString());
	if (!(p->hasNext()))
	{
		p = nullptr;
		QMessageBox::critical(nullptr, "Incorrect file", "File is incorrect or we can't render this scene");
	}
	else QMessageBox::information(nullptr, "Successfull", "File was added");
}

void Render_UI::render()
{
	if (p == nullptr)
	{
		QMessageBox::critical(nullptr, "Incorrect file", "Choose file with x3d scene before rendering");
		return;
	}
	CameraParam param = CameraParam(4, 90.0f);
	std::vector<BasicShape*> shapes;
	while (p->hasNext()) {
		shapes.push_back(p->NextElem());
	}
	Renderer rd = Renderer(param);
	rd.initScene(shapes);

	rd.cam.setLocation(Vector3(0,-1,7));
	
	rd.cam.Rotate(Quaternion(0, -1, 0));
	freopen("out_1.ppm", "wb", stdout);
	int h = 2040;
	int w = 1080;
	uint8_t header[] = "P6 2040 1080 255\n";
	header[17] = 10;
	uint8_t* image = (uint8_t*)malloc(h * w * 3 * sizeof(uint8_t));
	int32_t* ids = (int32_t*)malloc(h * w * sizeof(int32_t));
	std::cerr << rd.scene->getSize() << std::endl;

	//rd.printScene();

	rd.CPURenderPPMImage(&image, &ids, h, w);
	rd.scene->printBenchmark();

	fwrite(header, sizeof(uint8_t), sizeof(header) - 1, stdout);
	fwrite(image, sizeof(uint8_t), h * w * 3, stdout);
	freopen("im.txt", "w", stdout);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			std::cout << ids[i * w + j] << " ";
		}
		std::cout << "\n";
	}
	
	ui.image_lable->setPixmap(QPixmap("out_1.ppm"));

	free(image);
	free(ids);
}

void Render_UI::save() 
{

}