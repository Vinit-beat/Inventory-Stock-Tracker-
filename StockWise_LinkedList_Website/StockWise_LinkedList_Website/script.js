const KEY = "stockwise-linked-list-web-v1";
const demo = [
  {id:101,name:"Wireless Mouse",quantity:18,price:499},
  {id:102,name:"Mechanical Keyboard",quantity:4,price:1899},
  {id:103,name:"USB-C Hub",quantity:7,price:1299},
  {id:104,name:"Laptop Stand",quantity:3,price:899}
];
let products = loadProducts();
let editingId = null;
let toastTimer;
const $ = id => document.getElementById(id);

function loadProducts() {
  try {
    const saved = localStorage.getItem(KEY);
    return saved === null ? demo.map(p => ({...p})) : JSON.parse(saved);
  } catch (e) {
    return demo.map(p => ({...p}));
  }
}
function save() { localStorage.setItem(KEY, JSON.stringify(products)); }
function money(n) { return "₹" + Number(n).toLocaleString("en-IN",{minimumFractionDigits:2,maximumFractionDigits:2}); }
function safe(s) { return String(s).replace(/[&<>"']/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c])); }
function toast(message) {
  const el=$("toast"); el.textContent=message; el.classList.add("show");
  clearTimeout(toastTimer); toastTimer=setTimeout(()=>el.classList.remove("show"),2300);
}
function status(q) {
  if(q===0) return {label:"Out of stock",cls:"out"};
  if(q<=5) return {label:"Low stock",cls:"low"};
  return {label:"In stock",cls:"ok"};
}
function renderStats() {
  $("totalProducts").textContent=products.length;
  $("totalUnits").textContent=products.reduce((s,p)=>s+p.quantity,0);
  $("totalValue").textContent=money(products.reduce((s,p)=>s+p.quantity*p.price,0));
  $("lowStock").textContent=products.filter(p=>p.quantity<=5).length;
}
function renderList() {
  const area=$("linkedList"); area.innerHTML="";
  if(!products.length) {
    area.innerHTML='<div class="list-empty">List is empty. Add a product to create the HEAD node.</div>';
    $("listLength").textContent="0 nodes"; return;
  }
  products.forEach((p,i)=>{
    const node=document.createElement("div"); node.className="node";
    node.innerHTML=`<div class="node-head"><span>${i===0?"HEAD":"NODE "+(i+1)}</span><span>#${p.id}</span></div><div class="node-name" title="${safe(p.name)}">${safe(p.name)}</div><div class="node-meta">Qty: ${p.quantity} · ₹${p.price.toFixed(2)}</div>`;
    if(i<products.length-1) { const arrow=document.createElement("span"); arrow.className="pointer"; arrow.textContent="→"; node.append(arrow); }
    area.append(node);
  });
  const end=document.createElement("div"); end.className="null-node"; end.textContent="NULL"; area.append(end);
  $("listLength").textContent=products.length+(products.length===1?" node":" nodes");
}
function renderTable() {
  const q=$("searchInput").value.trim().toLowerCase();
  const filtered=products.filter(p=>String(p.id).includes(q)||p.name.toLowerCase().includes(q));
  const tbody=$("productRows"); tbody.innerHTML="";
  $("emptyState").classList.toggle("hidden",filtered.length!==0);
  filtered.forEach(p=>{
    const s=status(p.quantity), initials=p.name.trim().split(/\s+/).slice(0,2).map(w=>w[0]).join("").toUpperCase();
    const tr=document.createElement("tr");
    tr.innerHTML=`<td><div class="product"><div class="avatar">${safe(initials)}</div><div><div class="product-name">${safe(p.name)}</div><div class="sub">Inventory item</div></div></div></td>
      <td><span class="id-tag">#${p.id}</span></td><td><b>${p.quantity}</b> units</td><td><b>${money(p.price)}</b></td>
      <td><span class="status ${s.cls}">${s.label}</span></td>
      <td><button class="action-btn" data-action="edit" data-id="${p.id}">Edit</button><button class="action-btn delete" data-action="delete" data-id="${p.id}">Delete</button></td>`;
    tbody.append(tr);
  });
}
function render(){renderStats();renderList();renderTable();}
function resetForm(){
  editingId=null;$("productForm").reset();$("formTitle").textContent="Add a product";
  $("submitBtn").textContent="＋ Add to list";$("cancelEdit").classList.add("hidden");
  $("productId").disabled=false;
}
$("productForm").addEventListener("submit",e=>{
  e.preventDefault();
  const id=Number($("productId").value),name=$("productName").value.trim();
  const quantity=Number($("quantity").value),price=Number($("price").value);
  if(!Number.isInteger(id)||id<=0||!name||!Number.isInteger(quantity)||quantity<0||!Number.isFinite(price)||price<0){toast("Please enter valid product details.");return;}
  if(editingId===null&&products.some(p=>p.id===id)){toast("That product ID already exists.");return;}
  if(editingId!==null){
    const p=products.find(p=>p.id===editingId);if(!p)return;
    p.name=name;p.quantity=quantity;p.price=price;toast("Product updated.");
  } else {
    // Mirrors the C++ insertion: newNode->next = head; head = newNode;
    products.unshift({id,name,quantity,price});toast("New node inserted at HEAD.");
  }
  save();resetForm();render();
});
$("cancelEdit").addEventListener("click",resetForm);
$("searchInput").addEventListener("input",renderTable);
$("productRows").addEventListener("click",e=>{
  const b=e.target.closest("button[data-action]");if(!b)return;
  const id=Number(b.dataset.id),p=products.find(x=>x.id===id);if(!p)return;
  if(b.dataset.action==="edit"){
    editingId=id;$("formTitle").textContent="Edit product";$("submitBtn").textContent="Save changes";
    $("cancelEdit").classList.remove("hidden");$("productId").value=p.id;$("productId").disabled=true;
    $("productName").value=p.name;$("quantity").value=p.quantity;$("price").value=p.price;
    window.scrollTo({top:0,behavior:"smooth"});
  } else if(confirm(`Delete "${p.name}" (ID ${p.id}) from the list?`)){
    products=products.filter(x=>x.id!==id);if(editingId===id)resetForm();save();render();toast("Product node deleted.");
  }
});
$("resetDemo").addEventListener("click",()=>{
  if(!confirm("Restore the original demo products?"))return;
  products=demo.map(p=>({...p}));save();resetForm();$("searchInput").value="";render();toast("Demo inventory restored.");
});
render();
