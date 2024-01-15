#pragma once


#define VERTEXT_POSITION				0x01
#define VERTEXT_COLOR					0x02
#define VERTEXT_NORMAL					0x04
#define VERTEXT_TANGENT					0x08
#define VERTEXT_TEXTURE_COORD0			0x10
#define VERTEXT_TEXTURE_COORD1			0x20
#define VERTEXT_BONE_INDEX_WEIGHT		0x1000


class CMesh
{
public:
	CMesh() = default;
	CMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {};
	virtual ~CMesh() = default;
private:
	int m_nReferences = 0;
public:
	//����Ʈ ������ ���� �ʿ����.
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();
protected:
//--------�ٿ�� �ڽ� -------------
	XMFLOAT3 m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);

//----------------------���� ������ ���� ���� �� �ʿ�-------------------
protected:
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_nType = 0x00;
	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

//------------------------------���� ����-------------------------------
protected:
	vector<XMFLOAT3> m_pxmf3Positions;
	ComPtr<ID3D12Resource> m_pd3dPositionBuffer;
	ComPtr<ID3D12Resource> m_pd3dPositionUploadBuffer;
	
	vector<XMFLOAT3> m_pxmf3Normals;
	ComPtr<ID3D12Resource> m_pd3dNormalBuffer = NULL;
	ComPtr<ID3D12Resource> m_pd3dNormalUploadBuffer = NULL;

	//���⿡ ���� ���۵� vector�� �־� �Ѳ����� IA�� Set
	vector<D3D12_VERTEX_BUFFER_VIEW> m_pd3dVertexBufferViews; 

	UINT m_nVertexBuffers = 0;
//------------------------------�ε��� ����-------------------------------
protected:
	UINT m_nIndices = 0;
	vector<UINT> m_pnIndices;

	UINT m_nSubMeshes = 0;
	vector<UINT> m_pnSubSetIndices;
	vector<UINT> m_pnSubSetStartIndices;
	vector<vector<UINT>> m_ppnSubSetIndices;
	
	// �� Ȱ���� ���� ���ͷ� �ε��� ���� ���� - SubsetIndexBufferȰ�� 
	vector<ComPtr<ID3D12Resource>> m_ppd3dIndexBuffers;
	vector<ComPtr<ID3D12Resource>> m_ppd3dIndexUploadBuffers;
	vector<D3D12_INDEX_BUFFER_VIEW> m_pd3dIndexBufferViews;

	//�ε��� ���ۿ� ���ԵǴ� �ε����� �����̴�. 
	UINT m_nStartIndex = 0;
	//�ε��� ���ۿ��� �޽��� �׸��� ���� ���Ǵ� ���� �ε����̴�. 
	UINT m_nBaseVertex = 0;
	//�ε��� ������ �ε����� ������ �ε����̴�. 
public:
	vector<XMFLOAT3> GetPositions() { return m_pxmf3Positions; }
	vector<vector<UINT>> GetSubSetIndices() { return m_ppnSubSetIndices; }
	vector<UINT> GetIndices() { return m_pnIndices; }
public:
	//2024-01-15 �̼���
	//��ŷ ó���� ���� ���� ���庯ȯ �Լ�
	XMFLOAT3 TransformPoint(const XMFLOAT3& point, const XMFLOAT4X4& worldMatrix) const;
	virtual void IASetBeforeRender(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void RenderMesh(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT _nSubset);
	virtual void RenderMesh_OneIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	UINT GetType() { return(m_nType); }
};

//
class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual ~CTriangleMesh() { }
};
//
//class CCubeMeshDiffused : public CMesh
//{
//public:
//	//������ü�� ����, ����, ������ ���̸� �����Ͽ� ������ü �޽��� �����Ѵ�. 
//	CCubeMeshDiffused(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
//	virtual ~CCubeMeshDiffused();
//};