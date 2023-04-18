#include <rengine.hpp>
#include <cstdio>
#include <thread>
#include "SceneHierarchyPanel.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include "Rengine/Scene/Component.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Rengin
{

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
    m_Context = context;
    m_SelectionContext = {};
}

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    m_Context->m_registry.each([&](auto entityID)
    {
        Entity entity{entityID,m_Context.get()};
        
        DrawEntityNode(entity);
    });

    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionContext = {};

    //Right Click On Right Space
    if (ImGui::BeginPopupContextWindow(0,1,false))
    {
        if(ImGui::MenuItem("Create Empty Entity"))
            m_Context->CreateEntity("Empty Entity");
        ImGui::EndPopup();
    }
    

    ImGui::End();

    ImGui::Begin("Component");

    if (m_SelectionContext)
    {
        DrawComponents(m_SelectionContext);
        
    }
    
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity,flags,tag.c_str());

    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;
    
    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if(ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;
        ImGui::EndPopup();
    }

    if (opened)
        ImGui::TreePop();

    if(entityDeleted)
    {
        m_Context->DestroyEntity(entity);
        if (m_SelectionContext == entity)
            m_SelectionContext = {};
    }
}

static void DrawVec3Control(const std::string& label,glm::vec3& values,float resetValue = 0.f,float columnWidth = 100.f)
{
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0,columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2{0,0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImVec2 buttonSize = {lineHeight + 3.0f , lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.8f,0.1f,0.15f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.9f,0.2f,0.2f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.8f,0.1f,0.15f,1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X",buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X",&values.x,0.1f,-180.0f,180.0f,"%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.2f,0.7f,0.2f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.3f,0.8f,0.3f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.2f,0.7f,0.2f,1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y",buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Y",&values.y,0.1f,-180.0f,180.0f,"%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.1f,0.25f,0.8f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.2f,0.35f,0.9f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.1f,0.25f,0.8f,1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z",buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Z",&values.z,0.1f,-180.0f,180.0f,"%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
}

template<typename T,typename UIFunction>
static void DrawComponent(const std::string& name,Entity entity,UIFunction function)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth 
        | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if(entity.HasComponent<T>())
    {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2{4,4});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(),treeNodeFlags,name.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+",ImVec2{lineHeight,lineHeight}))
        {
            ImGui::OpenPopup("Component Setting");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Setting"))
        {
            if (ImGui::MenuItem("Remove Component"))
                removeComponent = true;
            ImGui::EndPopup();
        }
        
        if(open)
        {
            function(component);

            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<T>();
        
    }
}

void SceneHierarchyPanel::AddAttribute(std::vector<int>& v,std::map<Vec3<int>,Vec3<float>>& target,float x, float y, float z)
{
    // int size1 = v.size(),size2 = target.size();
    std::map<Vec3<int>,Vec3<float>> mp;
    for (int i = 0; i < v.size(); i += 3)
    {
        if(v[i] == -1)  continue;
        Vec3<int> loc;
        Vec3<float> val;
        loc[0] = v[i];
        loc[1] = v[i + 1];
        loc[2] = v[i + 2];
        val[0] = x;
        val[1] = y;
        val[2] = z;
        mp[loc] = val;

    }
    v.clear();
    for(auto& m : mp)
    {
        if(target.count(m.first) == 0)
        {
            target[m.first] = m.second;
        }else
        {
            auto& oriv = target[m.first];
            oriv[0] += m.second[0];
            oriv[1] += m.second[1];
            oriv[2] += m.second[2];
        }
    }
}

void SceneHierarchyPanel::ClearAttribute(std::vector<int>& v,std::map<Vec3<int>,Vec3<float>>& target,float x, float y, float z)
{
    std::map<Vec3<int>,Vec3<float>> mp;
    for (int i = 0; i < v.size(); i += 3)
    {
        if(v[i] == -1)  continue;
        Vec3<int> loc;
        Vec3<float> val;
        loc[0] = v[i];
        loc[1] = v[i + 1];
        loc[2] = v[i + 2];
        val[0] = x;
        val[1] = y;
        val[2] = z;
        mp[loc] = val;
    }
    v.clear();
    for(auto& m : mp)
    {
        target[m.first] = m.second;
    }
}

const int pick = 12;
const int dx[26] = {-1,0,1,-1,0,1,-1,0,1,-1,0,1,-1,1,-1,0,1,-1,0,1,-1,0,1,-1,0,1};
const int dy[26] = {-1,-1,-1,0,0,0,1,1,1,-1,-1,-1,0,0,1,1,1,-1,-1,-1,0,0,0,1,1,1};
const int dz[26] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1};
bool check_if_is_tooth(const std::vector<float>& label,int x,int y,int z,int w,int h,int d,std::unordered_map<int,int>& mp)
{
    for (int i = 0; i < 26; i++)
    {
        int id = (x + dx[i]) + w * (y + dy[i]) + w * h * (z + dz[i]);
        if(mp[label[id]] == 1)
        // if(label[id] == pick)
            return true;
    }
    return false;
}

void constraintVis(Ref<Texture3D> model, ForceComponent& force,
                ConstraintComponent& constraint, int width, int height,int depth)
{
    
    auto tex = model->getTexture();
    int hw = height * width;
    float maxval = 0.0f;
    float minval = 10000.0f;

	int idx = 0;
    auto showdata = tex;
    for(auto& itc:constraint.constraint)
	{
        showdata[itc.first[2] * hw + width * itc.first[1] + itc.first[0]] = 100;
    }	
    for(auto& itf:force.force)
	{
        showdata[itf.first[2] * hw + width * itf.first[1] + itf.first[0]] = 200;
    }

    
    RawWriter showWriter(showdata,width,height,depth);
    showWriter.write("D:/1u/constraint");
}

void GeneratePick(Ref<Texture3D> model)
{
    // std::string greyscale = "D:/1u/27178_20230307175424gray_584_584_427_int16.raw";
    // RawReader greyreader(greyscale);
    // auto width = greyreader.dimensions[0];
    // auto height = greyreader.dimensions[1];
    // auto depth = greyreader.dimensions[2];
    
    // int hw = height * width;
    // float maxval = 0.0f;
    // float minval = 10000.0f;

    // auto grey = greyreader.load();
    
    std::string show = "D:/2u3l/l/27724_20230308174058gray_474_584_584_int16.raw";
    RawReader reader(show);
    
    auto lwidth = reader.dimensions[0];
    auto lheight = reader.dimensions[1];
    auto ldepth = reader.dimensions[2];
    
    int lhw = lheight * lwidth;
    float lmaxval = 0.0f;
    auto labeldata = model->getTexture();

    auto data = reader.load();
    std::unordered_map<int,int> mp;
    mp[2] = 1;
    mp[27] = 1;
	int idx = 0;
    decltype(labeldata) showdata(labeldata.size());
    for (int k = 0; k < ldepth; k++)
	{
		for (int j = 0; j < lheight; j++)
		{
			for (int i = 0; i < lwidth; i++)
			{
                if(mp[labeldata[idx]])
                {
                    showdata[idx] = data[idx];
                }
                idx++;
			}
		}
    }

    RawWriter showWriter(showdata,lwidth,lheight,ldepth);
    showWriter.write("D:/1u/pick");
}


void GeneratePickResult(Ref<Texture3D> model,Ref<Texture3D> result)
{
    auto labeldata = model->getTexture();
    auto data = result->getTexture();
    auto lwidth = model->getWidth();
    auto lheight = model->getHeight();
    auto ldepth = model->getDepth();
    auto lwidth2 = result->getWidth();
    auto lheight2 = result->getHeight();
    auto ldepth2 = result->getDepth();
    int lhw = lheight * lwidth;
    std::unordered_map<int,int> mp;
    mp[1] = 2;
    mp[13] = 1;

	int idx = 0;
    decltype(labeldata) showdata(labeldata.size());
    for (int k = 0; k < ldepth; k++)
	{
		for (int j = 0; j < lheight; j++)
		{
			for (int i = 0; i < lwidth; i++)
			{
                
                // showdata[idx] = data[idx];
                // showdata[idx] = std::abs(data[idx * 3]);
                // showdata[idx] = std::sqrt(data[idx * 3] * data[idx * 3] + data[idx * 3 + 1] * data[idx * 3 + 1] + data[idx * 3 + 2] * data[idx * 3 + 2]);
                if(mp[labeldata[idx]])
                {
                    // showdata[idx] = std::abs(data[idx * 3]);
                    showdata[idx] = std::sqrt(data[idx * 3] * data[idx * 3] + data[idx * 3 + 1] * data[idx * 3 + 1] + data[idx * 3 + 2] * data[idx * 3 + 2]);
                }
                idx++;
			}
		}
    }

    RawWriter showWriter(showdata,lwidth,lheight,ldepth);
    showWriter.write("D:/1u/resultpick");
}

void GenerateMo(Ref<Texture3D> model)
{
    std::string greyscale = "D:/2u3l/u/27724_20230308174058gray_474_584_584_int16.raw";
    // std::string greyscale = "D:/1u/27178_20230307175424gray_584_584_427_int16.raw";
    RawReader greyreader(greyscale);
    auto width = greyreader.dimensions[0];
    auto height = greyreader.dimensions[1];
    auto depth = greyreader.dimensions[2];
    
    int hw = height * width;
    float maxval = 0.0f;
    float minval = 10000.0f;

    auto grey = greyreader.load();
    
    // std::unordered_set<double> sset;
    // std::string pp = "temp/material2.txt";
    // FILE *f = fopen(pp.c_str(), "w");
    // RE_CORE_ASSERT(f, "Cant Open the file");
    // fprintf(f,"0 13720000000 0.3 1\n");
    // fprintf(f,"1 25480000000 0.3 1\n");
    // fprintf(f,"2 48000000 0.49 1\n");
    // fclose(f);
    auto lwidth = model->getWidth();
    auto lheight = model->getHeight();
    auto ldepth = model->getDepth();
    std::unordered_map<int,int> mp;
    mp[13] = 1;
    mp[1] = 2;
    int lhw = lheight * lwidth;
    float lmaxval = 0.0f;

    auto labeldata = model->getTexture();

	int idx = 0;
    decltype(labeldata) showdata(labeldata.size());

    for (int k = 0; k < ldepth; k++)
	{
		for (int j = 0; j < lheight; j++)
		{
			for (int i = 0; i < lwidth; i++)
			{
                if(mp[labeldata[idx]] == 1)
                {
                    // showdata[idx] = 2;
                    maxval = std::max(maxval,grey[idx]);
                    minval = std::min(minval,grey[idx]);
                    double pho = -13.4 + 1017 * grey[idx];
                    double ym = -388.8 + 5925 * pho;
                    // showdata[idx] = ym;
                    showdata[idx] = grey[idx];
                    // sset.insert(ym);
                }
                else if(mp[labeldata[idx]] == 2)
                {
                    if(check_if_is_tooth(labeldata,i,j,k,lwidth,lheight,ldepth,mp))
                    {
                        showdata[idx] = 3;
                        // double ym = 48000000;
                        double ym = 5000;
                        // showdata[idx] = grey[idx];
                        showdata[idx] = ym;
                    }else
                    {
                        // showdata[idx] = 1;
                        double pho = -13.4 + 1017 * grey[idx];
                        double ym = -388.8 + 5925 * pho;
                        showdata[idx] = grey[idx];
                        // sset.insert(ym);
                    }
                    // maxval = std::max(maxval,grey[idx]);
                }
                idx++;
			}
		}
    }
    // int ii = 0;
    // for(auto& ss:sset)
    // {
    //     if(ss > 0)
    //     fprintf(f,"%d %lf 0.3 1\n",ii++,ss);
    // }
    // fclose(f);

    RawWriter showWriter(showdata,lwidth,lheight,ldepth);
    showWriter.write("D:/1u/mo");
}

void TransferMo(Ref<Texture3D> model,std::vector<float>& outdata)
{

    auto lwidth = model->getWidth();
    auto lheight = model->getHeight();
    auto ldepth = model->getDepth();
    
    int lhw = lheight * lwidth;
    float lmaxval = 0.0f;

    auto labeldata = model->getTexture();
    std::unordered_map<int,int> mp;
    mp[13] = 2;
    mp[12] = 1;
	int idx = 0;
    for (int k = 0; k < ldepth; k++)
	{
		for (int j = 0; j < lheight; j++)
		{
			for (int i = 0; i < lwidth; i++)
			{
                if(mp[labeldata[idx]] == 1)
                {
                    outdata[idx] = 2;
                }
                else if(mp[labeldata[idx]] == 2)
                {
                    if(check_if_is_tooth(labeldata,i,j,k,lwidth,lheight,ldepth,mp))
                    {
                        outdata[idx] = 3;
                    }else
                    {
                        outdata[idx] = 1;
                    }
                }
                idx++;
			}
		}
    }
    
    RawWriter showWriter(outdata,lwidth,lheight,ldepth);
    showWriter.write("D:/1u/newmodel");
}


void SceneHierarchyPanel::SaveIntFile(Ref<Texture3D> model, ForceComponent& force,
                ConstraintComponent& constraint, int width, int height,int depth)
{ 
    auto &texComR = m_VolomeEntity.GetComponent<ResultComponent>();
    // GeneratePickResult(model,texComR.Texture);
    // GenerateMo(model);
    // GeneratePick(model);
    std::string path = "./temp/";
    // constraintVis(model,force, constraint, width,  height, depth);
    // material
    auto pp = path + "material.txt";
    FILE *f = fopen((path + "material.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");
    fprintf(f,"0 13720000000 0.3 1\n");
    fprintf(f,"1 25480000000 0.3 1\n");
    fprintf(f,"2 48000000 0.49 1\n");
    fclose(f);

    // model
    f = fopen((path + "model.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");

    auto tex = model->getTexture();
    std::decay_t<decltype(tex)> showdata(tex.size());
    TransferMo(model,showdata);
	int idx = 0;
    int nums = 0;
    for (int k = 0; k < depth; k++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
                if(showdata[idx] > 0)
                    fprintf(f,"%d %d %d %d %d\n",idx,(int)showdata[idx] - 1,i,j,k),nums++;
                idx++;
			}
		}
    }

    fclose(f);

    // Bound Condition
    auto forc = force.force;
    auto cons = constraint.constraint;
    f = fopen((path + "constraint.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");
	idx = 0;
    int cnt = 0;
	for (auto itc = cons.begin(); itc != cons.end(); itc++)
	{
        fprintf(f, "SELECT_NODE_3D %d %d %d %f %f %f %d %d %d\n", itc->first[0], itc->first[1], itc->first[2],0,0,0, (int)itc->second[0], (int)itc->second[1], (int)itc->second[2]);
    }	
    for (auto itf = forc.begin(); itf != forc.end(); itf++)
	{
        fprintf(f, "SELECT_NODE_3D %d %d %d %f %f %f %d %d %d\n", itf->first[0], itf->first[1], itf->first[2], itf->second[0], itf->second[1], itf->second[2],0,0,0);
    }

    fclose(f);

    // Command
    f = fopen((path + "vo.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");
    fprintf(f,"SET_SCRIPT_VERSION 2\nSET_VOXEL_SIZE %f %f %f %f\nLOAD_MATERIALS temp/material.txt\nLOAD_MODEL %d %d %d %d temp/model.txt\nSET_TOLERANCE 1e-9\nSET_MAX_ITER 10000\nSET_ALGORITHM_FEA 1 1\nSELECTION_OF_NODES\nLOAD_CONSTRAINTS temp/constraint.txt\nSELECT_NODE_3D\nPRESERVE_NODE_3D\nCOMPUTE_SED\nSOLVE\nPRINT_DISPLACEMENTS temp/displacements.txt\nFINISH",0.3,0.3,0.3,1.0,width,height,depth,nums);
    fclose(f);
    // auto wpro = ExternalExe::Create();
    // wpro->CreateProcess("");
    // wpro->WaitProcess();
    // wpro->Terminate();
}

Ref<Texture3D> Coarsen(Ref<Texture3D> texture,int divided)
{
    auto& oldTex = texture->getTexture();
    uint32_t nWidth = texture->getWidth() / divided,nHeight = texture->getHeight() / divided,nDepth = texture->getDepth() / divided,bpp = texture->getBPP();
    std::decay_t<decltype(oldTex)> newTex(nWidth * nHeight * nDepth * bpp);
    int id = 0;
    int id2 = 0;
    int width = texture->getWidth(),height = texture->getHeight();
    for (int i = 0,ii = 0; i < nDepth; i++, ii += divided)
    {
        for (int j = 0,jj = 0; j < nHeight; j++, jj += divided)
        {
            for (int k = 0,kk = 0; k < nWidth; k++, kk += divided)
            {
                for (int l = 0; l < bpp; l++)
                {
                    std::unordered_map<int,int> mp;
                    for (int iii = 0; iii < divided; iii++)
                    {
                        for (int jjj = 0; jjj < divided; jjj++)
                        {
                            for (int kkk = 0; kkk < divided; kkk++)
                            {
                                id2 = bpp * ((ii + iii) * width * height + (jj + jjj) * width + kk + kkk);
                                mp[oldTex[id2 + l]]++;
                            }
                        }
                    }
                    int mostNumber,maxt = 0;
                    for (auto &&kv : mp)
                    {
                        if(kv.second > maxt)
                        {
                            mostNumber = kv.first;
                            maxt = kv.second;
                        }else if(kv.second == maxt /* && kv.first != 0*/)
                        {
                            mostNumber = std::min(mostNumber,kv.first);
                        }
                    }
                    if(mostNumber > 0)
                    {
                        int dasd = 0;
                    }
                    newTex[id + l] = mostNumber;
                }
                id ++;
            }
        }
    }

    auto newTexture = Texture3D::Create(nWidth,nHeight,nDepth,bpp);
    newTexture->setData(newTex);
    return newTexture;
}

#define MAX_LINE 1024

std::pair<Ref<Texture3D>,float> LoadStress(int width,int height,int depth,int scale,Ref<Texture3D> tex)
{
    FILE* f = fopen("temp/stress.txt", "r");
	if (!f)
	{
		printf("文件打开失败\n");
		return {0,0};
	}
	char buf[512];
	fgets(buf, MAX_LINE, f);
	fgets(buf, MAX_LINE, f);
	int maxX, maxY, maxZ;
	sscanf(buf, "Nodes: %d  %d  %d", &maxX, &maxY, &maxZ);
	fgets(buf, MAX_LINE, f);
    maxZ--;
    maxY--;
    maxX--;
	std::vector<float> field(maxX * maxY * maxZ);
    int id = 0;
	int num, x, y, z;
	double v, sx,sy,sz,sxy,syz,szx;
    double maxx,maxy,maxz,maxd = 0;
	while (fgets(buf, MAX_LINE, f) != NULL)
	{
		sscanf(buf, "%d   %d   %d   %d   %lf   %lf   %lf   %lf   %lf   %lf  ", &num, &x, &y, &z, &v, &sx, &sy,&sz, &sxy, &syz,&szx);
        id = x + maxX * y + maxX * maxY * z;
        id *= 3;
        if(x >= maxX || y >= maxY || z >= maxZ)
            continue;
		field[id++] = v;
        maxd = std::max(maxd,v);
		//field[x + maxX * y + maxX * maxY * z] = 1000;
	}

	fclose(f);
    auto texture = Texture3D::Create(maxX,maxY,maxZ,3,0x2601);
    texture->setData(field);

    return {texture,maxd};
}

std::pair<Ref<Texture3D>,float> LoadResult(int width,int height,int depth,int scale,Ref<Texture3D> tex)
{
    FILE* f = fopen("temp/displacements.txt", "r");
	if (!f)
	{
		printf("文件打开失败\n");
		return {0,0};
	}
	char buf[512];
	fgets(buf, MAX_LINE, f);
	fgets(buf, MAX_LINE, f);
	int maxX, maxY, maxZ;
	sscanf(buf, "Nodes: %d  %d  %d", &maxX, &maxY, &maxZ);
	fgets(buf, MAX_LINE, f);
    maxZ--;
    maxY--;
    maxX--;
	std::vector<float> field(maxX * maxY * maxZ * 3);
	std::vector<float> origin(width * height * depth * 3);
    int id = 0;
	int num, x, y, z;
	double dx, dy, dz;
    float maxx,maxy,maxz,maxd = 0;
	while (fgets(buf, MAX_LINE, f) != NULL)
	{
		sscanf(buf, "%d   %d   %d   %d   %lf   %lf   %lf  ", &num, &x, &y, &z, &dx, &dy, &dz);
        id = x + maxX * y + maxX * maxY * z;
        id *= 3;
        if(x >= maxX || y >= maxY || z >= maxZ)
            continue;
		field[id++] = dx;
		field[id++] = dy;
		field[id++] = dz;
        maxd = std::max(maxd,static_cast<float>(std::sqrt(dx * dx + dy * dy + dz * dz)));
		//field[x + maxX * y + maxX * maxY * z] = 1000;
	}
    auto temp = tex->getTexture();
    for (int i = 0; i < depth; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < width; k++)
            {
                id = k + width * j + width * height * i;
                if(temp[id] == 0)
                {
                    continue;
                }
                id *= 3;
                
                int id2 = k + maxX * j + maxX * maxY * i;
                id2 *= 3;
                id2 /= scale;
                origin[id++] = field[id2++];
                origin[id++] = field[id2++];
                origin[id++] = field[id2++];
            }
        }
    }
    
	fclose(f);
    auto texture = Texture3D::Create(maxX,maxY,maxZ,3,0x2601);
    texture->setData(field);
    // auto texture = Texture3D::Create(width,height,depth,3);
    // texture->setData(origin);
    return {texture,maxd};
}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    if(entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        char buffer[256];
        memset(buffer,0,sizeof(buffer));
        strcpy_s(buffer,sizeof(buffer),tag.c_str());
        if(ImGui::InputText("Tag",buffer,sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);


    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (ImGui::MenuItem("Camera"))
        {
            m_SelectionContext.AddComponent<CameraComponent>();
            ImGui::CloseCurrentPopup();
        }
        
        if (ImGui::MenuItem("Sprite Render"))
        {
            m_SelectionContext.AddComponent<SpriteRendererComponent>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform",entity,[](auto& component)
    {
        DrawVec3Control("Translation",component.Translation);
        glm::vec3 rotation = glm::degrees(component.Rotation);
        DrawVec3Control("Rotation",rotation);
        component.Rotation = glm::radians(rotation);
        DrawVec3Control("Scale",component.Scale,1.0f);
    });

    DrawComponent<CameraComponent>("Camera",entity,[](auto& component)
    {
        auto& camera = component.Camera;

        ImGui::Checkbox("Primary",&component.Primary);

        const char* prjectionTypeStrings[] = {"Perspective","Orthographics"};
        const char* currentProjectionTypeString = prjectionTypeStrings[(int)camera.GetProjectionType()];

        if(ImGui::BeginCombo("Projection",currentProjectionTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentProjectionTypeString == prjectionTypeStrings[i];
                if(ImGui::Selectable(prjectionTypeStrings[i],isSelected))
                {
                    currentProjectionTypeString = prjectionTypeStrings[i];
                    camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
                }
                if(isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
        {
            float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
            if(ImGui::DragFloat("FOV",&perspectiveFOV,0.1,30.0f,90.0f,"%.2f"))
                camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));

            float perspectiveNearClip = camera.GetPerspectiveNearClip();
            if(ImGui::DragFloat("Near",&perspectiveNearClip))
                camera.SetPerspectiveNearClip(perspectiveNearClip);

            float perspectiveFarClip = camera.GetPerspectiveFarClip();
            if(ImGui::DragFloat("Far",&perspectiveFarClip))
                camera.SetPerspectiveFarClip(perspectiveFarClip);
        }
        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographics)
        {
            float orthoSize = camera.GetOrthographicsSize();
            if(ImGui::DragFloat("Size",&orthoSize))
                camera.SetOrthographicsSize(orthoSize);

            float orthoNearClip = camera.GetOrthographicsNearClip();
            if(ImGui::DragFloat("Near",&orthoNearClip))
                camera.SetOrthographicsNearClip(orthoNearClip);

            float orthoFarClip = camera.GetOrthographicsFarClip();
            if(ImGui::DragFloat("Far",&orthoFarClip))
                camera.SetOrthographicsFarClip(orthoFarClip);

            ImGui::Checkbox("Fixed Aspect Ratio",&component.FixedAspectRatio);
        }
    });


    DrawComponent<SpriteRendererComponent>("Sprite Renderer",entity,[](auto& component)
    {
        ImGui::ColorEdit4("Color",glm::value_ptr(component.Color));
    });
    
    DrawComponent<Texture3DComponent>("Texture3D",entity,[this](auto& component)
    {
        ImGui::Text(component.Path.c_str());
        ImGui::Text("width: %d, height: %d, depth: %d",component.width,component.height,component.depth);
        ImGui::DragFloat("threshold",&component.Threshold,0.001,0.0f,1.0f,"%.2f");
        if(ImGui::Button("Coarsen"))
        {
            if(component.inVscale = 1)
            {
                component.Origin = component.Texture;
                component.Owidth= component.width;
                component.Oheight= component.height;
                component.Odepth= component.depth;
            }
            auto newTexture = Coarsen(component.Texture,2);
            component.inVscale *= 2;
            component.Texture = newTexture;
            component.width /= 2;
            component.height /= 2;
            component.depth /= 2;

            auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
            texComF.force.clear();
            // std::decay_t<decltype(texComF.Texture->getTexture())> newForce(component.width * component.height * component.depth * texComF.Texture->getBPP());
            // auto newTexF = Texture3D::Create(component.width,component.height,component.depth,texComF.Texture->getBPP());
            // newTexF->setData(newForce);
            // texComF.Texture = newTexF;
            
            auto& texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            texComC.constraint.clear();
            // std::decay_t<decltype(texComC.Texture->getTexture())> newConstraint(component.width * component.height * component.depth * texComC.Texture->getBPP());
            // auto newTexC = Texture3D::Create(component.width,component.height,component.depth,texComF.Texture->getBPP());
            // newTexC->setData(newConstraint);
            // texComC.Texture = newTexC;
        }
        ImGui::SameLine();
        if(ImGui::Button("Save New Texture"))
        {

        }
        if(ImGui::Button("Show Texture"))
        {
            auto &texComR = m_VolomeEntity.GetComponent<ResultComponent>();
            texComR.showId = 1;
        }
    });

    DrawComponent<ForceComponent>("Force",entity,[this](auto& component)
    {
        ImGui::Text("Forces");
        char buffer[25];
        memset(buffer,0,sizeof(buffer));
        static float fx = 0,fy = 0,fz = 0;
        sprintf(buffer,"%f",fx);
        if(ImGui::InputText("Fx",buffer,sizeof(buffer)))
        {
            fx = atof(buffer);
        }
        sprintf(buffer,"%f",fy);
        if(ImGui::InputText("Fy",buffer,sizeof(buffer)))
        {
            fy = atof(buffer);
        }
        sprintf(buffer,"%f",fz);
        if(ImGui::InputText("Fz",buffer,sizeof(buffer)))
        {
            fz = atof(buffer);
        }
        if(ImGui::Button("Add Force"))
        {
            auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
            AddAttribute(m_PickedPixels,texComF.force,fx,fy,fz);
            // texComF.Texture->setData(texComF.Texture->getTexture().data(),texComF.Texture->getTexture().size());
            fx = fy = fz = 0;
        }
        if(ImGui::Button("Clear Force"))
        {
            auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
            fx = fy = fz = 0;
            ClearAttribute(m_PickedPixels,texComF.force,fx,fy,fz);
            // texComF.Texture->setData(texComF.Texture->getTexture().data(),texComF.Texture->getTexture().size());
        }
        if(ImGui::Button("Show Force"))
        {
            auto &texComR = m_VolomeEntity.GetComponent<ResultComponent>();
            texComR.showId = 2;
        }
    });

    DrawComponent<ConstraintComponent>("Constraint",entity,[this](auto& component)
    {
        ImGui::Text("Constraint");
        if(ImGui::Button("Add x"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.constraint,1,0,0);
            // texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        ImGui::SameLine();
        if(ImGui::Button("Add y"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.constraint,0,1,0);
            // texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        ImGui::SameLine();
        if(ImGui::Button("Add z"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.constraint,0,0,1);
            // texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        ImGui::SameLine();
        if(ImGui::Button("Add Constraint"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.constraint,1,1,1);
            // texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        if(ImGui::Button("Clear Constraint"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            ClearAttribute(m_PickedPixels,texComC.constraint,0,0,0);
            // texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        if(ImGui::Button("Show Constraint"))
        {
            auto &texComR = m_VolomeEntity.GetComponent<ResultComponent>();
            texComR.showId = 3;
        }
    });
    DrawComponent<SolveComponent>("Solve!", entity, [this](auto &component) {
            if (ImGui::Button("Solve")) {
                auto& texCom = m_VolomeEntity.GetComponent<Texture3DComponent>();
                auto& texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
                auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
                // std::thread t(&SceneHierarchyPanel::SaveIntFile,this,texCom.Texture,texComF.Texture,texComC.Texture,texCom.width,texCom.height,texCom.depth);
                SaveIntFile(texCom.Texture,texComF,texComC,texCom.width,texCom.height,texCom.depth);
                // t.detach();
                // m_ExternalProcess->CreateProcess("./femsolver.exe","  temp/vo.txt");
                // m_ExternalProcess->WaitProcess();
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop")) {
                m_ExternalProcess->Terminate();
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Result")) {
                auto& texCom = m_VolomeEntity.GetComponent<Texture3DComponent>();
                auto &texComR = m_VolomeEntity.GetComponent<ResultComponent>();
                auto [tex,value] = LoadResult(texCom.width,texCom.height,texCom.depth,4,texCom.Texture); 
                texComR.Texture = tex;
                texComR.maxvalue[3] = value;
                texComR.showId = 4;
            }
            ImGui::SameLine();
            if (ImGui::Button("Show Result")) {
                auto &texComR = m_VolomeEntity.GetComponent<ResultComponent>();
                texComR.showId = 4;
            }
        });
        DrawComponent<ToothChooseComponent>("Choose", entity, [this](auto &component) {
            if (ImGui::Button("tooth")) {
                if(component.Choose & 1)
                    component.Choose &= ~1;
                else
                    component.Choose |= 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("alveolar bone")) {
                if(component.Choose & 2)
                    component.Choose &= ~2;
                else
                    component.Choose |= 2;
            }
            ImGui::SameLine();
            if (ImGui::Button("periodontal ligament")) {
                if(component.Choose & 4)
                    component.Choose &= ~4;
                else
                    component.Choose |= 4;
            }
        });
    DrawComponent<ColorTransferFunctionComponent>(
        "ColorTransferFunction", entity, [](auto &component) {
          auto &transferFun = component.Color;
          ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses
          ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to
          glm::vec2 clip_board{canvas_sz.x, 10};
          canvas_sz.y = clip_board.y;
          ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
          ImGuiIO &io = ImGui::GetIO();
          ImDrawList *draw_list = ImGui::GetWindowDrawList();
          draw_list->AddRectFilled(canvas_p0, canvas_p1,
                                   IM_COL32(50, 50, 150, 255));
          draw_list->AddRect(canvas_p0, canvas_p1,
                             IM_COL32(255, 255, 255, 255));

          ImGui::InvisibleButton("Color TransferFunction", canvas_sz,
                                 ImGuiButtonFlags_MouseButtonLeft |
                                     ImGuiButtonFlags_MouseButtonRight);
          const bool is_hovered = ImGui::IsItemHovered(); // Hovered
          const bool is_active = ImGui::IsItemActive();   // Held
          const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
          const glm::vec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                              io.MousePos.y - origin.y);

          static int old_key = -1;
          if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            old_key = mouse_pos_in_canvas.x;
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x);
          }
          if (is_active && old_key!= -1 &&
              ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0)) {
            if (old_key != mouse_pos_in_canvas.x)
            {
              transferFun.Erase(old_key / clip_board.x);
              old_key = mouse_pos_in_canvas.x;
            }
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x);
          }
          draw_list->PushClipRect(canvas_p0, canvas_p1, true);

          for (auto it = transferFun.begin(); it != transferFun.end(); it++) {
            draw_list->AddCircleFilled(
                ImVec2(origin.x + it->first * clip_board.x,origin.y + 5.0f), 5.0f,
                IM_COL32(255, 0, 0, 255));
          }
          draw_list->PopClipRect();

          if (ImGui::Button("clear Color", {clip_board.x / 2, 20})) {
                transferFun.Reset();
          }
          ImGui::SameLine();
          if (ImGui::Button("delete Color", {clip_board.x / 2, 20})) {
            if (old_key != -1) {
              transferFun.Erase(old_key / clip_board.x);
              old_key = -1;
            }
          }
        });

    DrawComponent<OpacityTransferFunctionComponent>(
        "OpacityTransferFunction", entity, [](auto &component) {
          auto &transferFun = component.Opacity;
          ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses
          ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to
          glm::vec2 clip_board{canvas_sz.x, 100.0f};
          canvas_sz.y = clip_board.y;
          ImVec2 canvas_p1 =
              ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
          ImGuiIO &io = ImGui::GetIO();
          ImDrawList *draw_list = ImGui::GetWindowDrawList();
          draw_list->AddRectFilled(canvas_p0, canvas_p1,
                                   IM_COL32(50, 50, 150, 255));
          draw_list->AddRect(canvas_p0, canvas_p1,
                             IM_COL32(255, 255, 255, 255));

          ImGui::InvisibleButton("Opacity TransferFunction", canvas_sz,
                                 ImGuiButtonFlags_MouseButtonLeft |
                                     ImGuiButtonFlags_MouseButtonRight);
          const bool is_hovered = ImGui::IsItemHovered(); // Hovered
          const bool is_active = ImGui::IsItemActive();   // Held
          const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
          const glm::vec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                              io.MousePos.y - origin.y);

          static int old_key = -1;
          if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            old_key = mouse_pos_in_canvas.x;
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x,1.0f - mouse_pos_in_canvas.y / clip_board.y);

          }
          if (is_active && old_key!= -1 &&
              ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0)) {
            if (old_key != mouse_pos_in_canvas.x)
            {
              transferFun.Erase(old_key / clip_board.x);
              old_key = mouse_pos_in_canvas.x;
            }
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x,1.0f - mouse_pos_in_canvas.y / clip_board.y);
          }
          draw_list->PushClipRect(canvas_p0, canvas_p1, true);

          for (auto it = transferFun.begin(); it != transferFun.end(); it++) {
            draw_list->AddCircleFilled(
                ImVec2(origin.x + it->first * clip_board.x, origin.y + (1.0f - it->second) * clip_board.y), 5.0f,
                IM_COL32(255, 0, 0, 255));

            auto temp = it;
            temp++;

            if (temp != transferFun.end())
              draw_list->AddLine(
                  ImVec2(origin.x + it->first * clip_board.x, origin.y + (1.0f - it->second) * clip_board.y),
                  ImVec2(origin.x + temp->first * clip_board.x, origin.y + (1.0f - temp->second) * clip_board.y),
                  IM_COL32(200, 200, 200, 40));
          }
          draw_list->PopClipRect();

          if (ImGui::Button("clear Opacity", {clip_board.x / 2, 20})) {
            transferFun.Reset();
          }
          ImGui::SameLine();
          if (ImGui::Button("delete Opacity", {clip_board.x / 2, 20})) {
            if (old_key != -1) {
              transferFun.Erase(old_key / clip_board.x);
              old_key = -1;
            }
          }
        });
}

} // namespace Rengin
